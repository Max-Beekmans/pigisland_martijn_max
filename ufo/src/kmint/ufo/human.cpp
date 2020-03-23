#include <kmint/ufo/building.hpp>
#include <iostream>
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/door.hpp"
#include "kmint/random.hpp"

namespace kmint::ufo {

namespace {
constexpr char const *image_path = "resources/human.png";

graphics::image human_image() { return graphics::image{image_path}; }

} // namespace
human::human(math::vector2d location)
	: play::free_roaming_actor{ location }, dna_entity(),
	drawable_{ *this, human_image() }, velocity_{20}, isDead_{false}, isSafe_{false}, fitness_{200} {}

bool human::isDead() const {
    return isDead_;
}

bool human::isSafe() const {
    return isSafe_;
}

int human::getFitness() const {
    return fitness_;
}

void human::setFitness(int fitness) {
    fitness_ = fitness;
}

void human::setIsDead(bool isDead) {
    isDead_ = isDead;
}

void human::setIsSafe(bool isSafe) {
    isSafe_ = isSafe;
}

void human::act(delta_time dt) {
    math::vector2d forceVector = {0,0};
    math::vector2d alignmentVector = {0,0};
    math::vector2d cohesionVector = {0,0};
    math::vector2d separationVector = {0,0};
    math::vector2d collisionAvoidanceVector = {0,0};
    math::vector2d ufoVector = {0,0};
    math::vector2d redTankVector = {0,0};
    math::vector2d greenTankVector = {0,0};
    math::vector2d doorVector = {0,0};
    math::vector2d screenVector = {0,0};

    t_since_move_ += dt;

    if(to_seconds(t_since_move_) >= 1) {
        if(!isSafe_) setFitness(getFitness() - 1);

        randomVector_ = {random_scalar(-1, 1), random_scalar(-1, 1)};
        t_since_move_ = from_seconds(0);
    }

    int perceivedHumans = 0;
    int perceivedSaucers = 0;
    int perceivedDoors = 0;
    int perceivedBuildings = 0;
    for (std::size_t ix{}; ix < num_perceived_actors(); ++ix) {
        auto &other = perceived_actor(ix);

        //! Collect all the force vectors from other actors if check for type
        if (dynamic_cast<human *>(&other)) {
            //! add location of other human
            cohesionVector += { other.location() };
            //! add the heading of other human
            alignmentVector += other.heading();
            //! add force vector towards other human
            separationVector += {other.location().x() - location().x(), other.location().y() - location().y()};
            perceivedHumans++;
        }
        else if (dynamic_cast<saucer *>(&other)) {
            //! add force vector towards saucer
            ufoVector += {other.location().x() - location().x(), other.location().y() - location().y()};
            perceivedSaucers++;
        }
        else if(dynamic_cast<door *>(&other)) {
            //! add force vector towards door
            doorVector += {other.location().x() - location().x(), other.location().y() - location().y()};
            perceivedDoors++;
        }
        else if(dynamic_cast<building *>(&other)) {
            auto ahead = location() + heading() * velocity_ * to_seconds(dt);
            collisionAvoidanceVector += {ahead.x() - other.location().x(), ahead.y() - other.location().y()};
            perceivedBuildings++;
        }
        else if ( auto t = dynamic_cast<tank *>(&other); t) {
            if(t->type() == kmint::ufo::tank_type::red) {
                //! add force vector towards red tank
                redTankVector += {other.location().x() - location().x(), other.location().y() - location().y()};
                redTankVector = normalizeForce(redTankVector);

            }
            else if(t->type() == kmint::ufo::tank_type::green) {
                //! add force vector towards green tank
                greenTankVector += {other.location().x() - location().x(), other.location().y() - location().y()};
                greenTankVector = normalizeForce(greenTankVector);
            }
        }
    }

    if(perceivedHumans > 0) {
        //! devide by perceived humans to get the average heading
        alignmentVector /= perceivedHumans;
        //! normalize the vector
        alignmentVector = normalizeForce(alignmentVector);

        //! devide by perceived humans to get the position of the center of mass
        cohesionVector /= perceivedHumans;
        //! calculate the distance from this human towards the center of mass
        cohesionVector -= {location().x(), location().y()};
        //! normalize the vector
        cohesionVector = normalizeForce(cohesionVector);

        //! devide by perceived humans to get average force
        separationVector /= perceivedHumans;
        //! invert the vector because we collected forces towards others
        separationVector *= -1;
        //! normalize the vector
        separationVector = normalizeForce(separationVector);

    }

    if(perceivedSaucers > 0) {
        //! devide by perceived saucers to get average force
        ufoVector /= perceivedSaucers;
        //! normalize the vector
        ufoVector = normalizeForce(ufoVector);
    }

    if(perceivedDoors > 0){
        //! devide by perceived doors to get average force
        doorVector /= perceivedDoors;
        //! normalize the vector
        doorVector = normalizeForce(doorVector);
    }

    if(perceivedBuildings > 0) {
        collisionAvoidanceVector /= perceivedBuildings;
        collisionAvoidanceVector = normalizeForce(collisionAvoidanceVector);
    }

    //! Get the genetic attributes of the human
    auto geneticAttributes = getGeneticAttributes();
    //! Use the genetic attributes to multiply the forces and calculate a single force vector
    forceVector = redTankVector * geneticAttributes.getAttractionToRedTank() +
                  greenTankVector * geneticAttributes.getAttractionToGreenTank() +
                  ufoVector * geneticAttributes.getAttractionToUFO() + doorVector * geneticAttributes.getAttractionToDoors() +
                  cohesionVector * geneticAttributes.getCohesion() + separationVector * geneticAttributes.getSeparation() +
                  alignmentVector * geneticAttributes.getAlignment();
    //! normalize the vector
    forceVector = normalizeForce(forceVector);

    if(collidesWithScreen()) {
        //! calculte force towards the center of the screen
        screenVector += { 1024/2 - location().x(), 768/2 - location().y() };
        //! normalize the vector
        screenVector = normalizeForce(screenVector);
    }

    math::vector2d finalVector = forceVector + screenVector * 2 + collisionAvoidanceVector;
    finalVector += (finalVector == math::vector2d{0,0} ? randomVector_ : math::vector2d{0,0});
    finalVector = normalizeForce(finalVector);
    heading(finalVector);

    location(location() + velocity_ * heading() * to_seconds(dt));
}

//! normalize fails on a vector with a length of 0, we still want a force that goes in one direction to have effect
math::vector2d human::normalizeForce(math::vector2d force) const {
    if(force.x() != 0 && force.y() != 0) {
        return math::normalize(force);
    } else {
        return {static_cast<float>(force.x() > 0 ? 1 : 0), static_cast<float>(force.y() > 0 ? 1 : 0) };
    }
}

} // namespace kmint::ufo
