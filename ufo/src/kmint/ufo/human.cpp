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

math::vector2d random_location() {
  return {random_scalar(60, 900), random_scalar(60, 700)};
}

} // namespace
human::human(int humanCounter)
	: play::free_roaming_actor{ random_location() }, dna_entity(),
	drawable_{ *this, human_image() }, velocity_{20}, isDead_{false}, isSafe_{false}, humanCounter_{humanCounter}{
}

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

void human::setRandomLocation() {
    location(random_location());
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
    math::vector2d collisionVector = {0,0};
    math::vector2d ufoVector = {0,0};
    math::vector2d redTankVector = {0,0};
    math::vector2d greenTankVector = {0,0};
    math::vector2d doorVector = {0,0};
    math::vector2d screenVector = {0,0};

    t_since_move_ += dt;

    if(to_seconds(t_since_move_) >= 1) {
        if(!isDead_) setFitness(getFitness() + 1);

        randomVector_ = {random_scalar(-1, 1), random_scalar(-1, 1)};
        t_since_move_ = from_seconds(0);
    }

    int perceivedHumans = 0;
    int perceivedSaucers = 0;
    int perceivedDoors = 0;
    for (std::size_t ix{}; ix < num_perceived_actors(); ++ix) {
        auto &other = perceived_actor(ix);
        if (dynamic_cast<human *>(&other)) {
            cohesionVector += { other.location() };
            alignmentVector += other.heading();
            separationVector += {other.location().x() - location().x(), other.location().y() - location().y()};
            perceivedHumans++;
        }
        else if (dynamic_cast<saucer *>(&other)) {
            ufoVector += {other.location().x() - location().x(), other.location().y() - location().y()};
            perceivedSaucers++;
        }
        else if(dynamic_cast<door *>(&other)) {
            doorVector += {other.location().x() - location().x(), other.location().y() - location().y()};
            perceivedDoors++;
        }
        else if ( auto t = dynamic_cast<tank *>(&other)) {
            if(t->type() == kmint::ufo::tank_type::red) {
                redTankVector += {other.location().x() - location().x(), other.location().y() - location().y()};
                redTankVector = normalizeForce(redTankVector);

            }
            else if(t->type() == kmint::ufo::tank_type::green) {
                greenTankVector += {other.location().x() - location().x(), other.location().y() - location().y()};
                greenTankVector = normalizeForce(greenTankVector);
            }
        }
    }

    if(perceivedHumans > 0) {
        alignmentVector /= perceivedHumans;
        alignmentVector = normalizeForce(alignmentVector);

        cohesionVector /= perceivedHumans;
        cohesionVector -= {location().x(), location().y()};
        cohesionVector = normalizeForce(cohesionVector);

        separationVector /= perceivedHumans;
        separationVector *= -1;
        separationVector = normalizeForce(separationVector);

    }

    if(perceivedSaucers > 0) {
        ufoVector /= perceivedSaucers;
        ufoVector = normalizeForce(ufoVector);
    }

    if(perceivedDoors > 0){
        doorVector /= perceivedDoors;
        doorVector = normalizeForce(doorVector);
    }

    auto geneticAttributes = getGeneticAttributes();
    forceVector = redTankVector * geneticAttributes.getAttractionToRedTank() +
                  greenTankVector * geneticAttributes.getAttractionToGreenTank() +
                  ufoVector * geneticAttributes.getAttractionToUFO() + doorVector * geneticAttributes.getAttractionToDoors() +
                  cohesionVector * geneticAttributes.getCohesion() + separationVector * geneticAttributes.getSeparation() +
                  alignmentVector * geneticAttributes.getAlignment();

    forceVector = normalizeForce(forceVector);

    if(collidesWithScreen()) {
        screenVector += { 1024/2 - location().x(), 768/2 - location().y() };
        screenVector = normalizeForce(screenVector);
    }

    int collidedActors = 0;
    for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
        auto &other = colliding_actor(ix);
        if(dynamic_cast<door*>(&other)) {
            isSafe_ = true;
        }
        if(dynamic_cast<building*>(&other)) {
            std::cout << "collision with building" << std::endl;
        }
        collisionVector += { location().x() - other.location().x(), location().y() - other.location().y() };
        //collisionVector -= forceVector;
        collidedActors++;
    }

    if(collidedActors > 0) {
        collisionVector /= collidedActors;
        collisionVector = normalizeForce(collisionVector);
    }

    math::vector2d finalVector = forceVector + screenVector + collisionVector + (screenVector + collisionVector == math::vector2d{0,0} ? randomVector_ : math::vector2d{0,0});
    finalVector = normalizeForce(finalVector);
    heading(finalVector);

    location(location() + velocity_ * heading() * to_seconds(dt));
}

math::vector2d human::normalizeForce(math::vector2d force) const {
    if(force.x() != 0 && force.y() != 0) {
        return math::normalize(force);
    } else {
        return {static_cast<float>(force.x() > 0 ? 1 : 0), static_cast<float>(force.y() > 0 ? 1 : 0) };
    }
}

math::vector2d human::collisionWithBuildings() const {
    std::vector<math::rectangle> buildings;
    buildings.push_back({{576, 64}, {80, 48}}); //row 1, building 1
    buildings.push_back({{624, 64}, {112, 62}}); //row 1, building 2
    buildings.push_back({{576, 208}, {96, 112}}); //row 2, building 1
    buildings.push_back({{320, 512}, {112, 128}}); //row 3, building 1
    buildings.push_back({{432, 464}, {80, 160}}); //row 3, building 2
    buildings.push_back({{576, 400}, {112, 96}}); //row 3, building 3

    math::vector2d vector = {0,0};
    int counter = 0;

    for(std::vector<math::rectangle>::iterator it = buildings.begin(); it != buildings.end(); it++) {
        if(math::intersect(collision_box(), *it)){
            vector += { location().x() - (it->top_left().x() + it->size().width()/2 - 8), location().y() - (it->top_left().y() + it->size().height()/2 - 8)};
            counter++;
        }
    }

    if(counter > 0) vector /= counter;

    return normalizeForce(vector);
}

} // namespace kmint::ufo
