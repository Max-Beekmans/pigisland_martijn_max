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
human::human()
	: play::free_roaming_actor{ random_location() }, dna_entity(),
	drawable_{ *this, human_image() },
	velocity_{20}{
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

    int perceivedHumans = 0;
    int perceivedSaucers = 0;
    int perceivedDoors = 0;
    for (std::size_t ix{}; ix < num_perceived_actors(); ++ix) {
        auto &other = perceived_actor(ix);
        if (dynamic_cast<human *>(&other)) {
            cohesionVector += { other.location() };
            alignmentVector += other.heading();
            separationVector += {other.location() - location()};
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
                if (redTankVector.x() != 0 && redTankVector.y() != 0) {
                    redTankVector = math::normalize(redTankVector);
                } else {
                    redTankVector = {0, 0};
                }
            }
            else if(t->type() == kmint::ufo::tank_type::green) {
                greenTankVector += {other.location().x() - location().x(), other.location().y() - location().y()};
                if (greenTankVector.x() != 0 && greenTankVector.y() != 0) {
                    greenTankVector = math::normalize(greenTankVector);
                } else {
                    greenTankVector = {0, 0};
                }
            }
        }
    }

    if(perceivedHumans > 0) {
        alignmentVector /= perceivedHumans;
        if (alignmentVector.x() != 0 && alignmentVector.y() != 0) {
            alignmentVector = math::normalize(alignmentVector);
        } else {
            alignmentVector = {0, 0};
        }

        cohesionVector /= perceivedHumans;
        cohesionVector -= {location().x(), location().y()};
        if (cohesionVector.x() != 0 && cohesionVector.y() != 0) {
            cohesionVector = math::normalize(cohesionVector);
        } else {
            cohesionVector = {0, 0};
        }

        separationVector /= perceivedHumans;
        separationVector *= -1;
        if (separationVector.x() != 0 && separationVector.y() != 0) {
            separationVector = math::normalize(separationVector);
        } else {
            separationVector = {0, 0};
        }
    }

    if(perceivedSaucers > 0) {
        ufoVector /= perceivedSaucers;
        if(ufoVector.x() != 0 && ufoVector.y() != 0) {
            ufoVector = math::normalize(ufoVector);
        } else {
            ufoVector = {0,0};
        }
    }

    if(perceivedDoors > 0){
        doorVector /= perceivedDoors;
        if(doorVector.x() != 0 && doorVector.y() != 0) {
            doorVector = math::normalize(doorVector);
        } else {
            doorVector = {0,0};
        }
    }

    auto geneticAttributes = getGeneticAttributes();

    forceVector = redTankVector * geneticAttributes.getAttractionToRedTank() +
            greenTankVector * geneticAttributes.getAttractionToGreenTank() +
            ufoVector * geneticAttributes.getAttractionToUFO() + doorVector * geneticAttributes.getAttractionToDoors() +
            cohesionVector * geneticAttributes.getCohesion() + separationVector * geneticAttributes.getSeparation() +
            alignmentVector * geneticAttributes.getAlignment();

    //vector becomes the normalized vector of all the combined forces
    //cant normalize if x or y is 0
    if(forceVector.x() != 0 && forceVector.y() != 0){
        forceVector = math::normalize(forceVector);
    } else {
        forceVector = {0,0};
    }

    if(collidesWithScreen()) {
        screenVector += { 1024/2 - location().x(), 768/2 - location().y() };
        if(screenVector.x() != 0 && screenVector.y() != 0) {
            screenVector = math::normalize(screenVector);
        } else {
            screenVector = {0,0};
        }
    }

    int collidedActors = 0;
    for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
        auto &other = colliding_actor(ix);
        collisionVector += { location() - other.location() };
        collidedActors++;
    }

    if(collidedActors > 0) {
        collisionVector /= collidedActors;
        if(collisionVector.x() != 0 && collisionVector.y() != 0) {
            collisionVector = math::normalize(collisionVector);
        } else {
            collisionVector = {0,0};
        }
    }

    heading(forceVector + collisionVector + screenVector);

    location(location() + velocity_ * heading() * to_seconds(dt));
    setFitness(getFitness() + 1);
}

} // namespace kmint::ufo
