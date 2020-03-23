#include <string>
#include <iostream>
#include <kmint/random.hpp>
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/ufowanderstate.h"
#include "kmint/ufo/forzenstate.h"

namespace kmint::ufo {

    namespace {

        char const *color_for(saucer_type type) {
            switch (type) {
                case saucer_type::blue:
                    return "Blue";
                case saucer_type::green:
                    return "Green";
                case saucer_type::beige:
                    return "Beige";
                case saucer_type::yellow:
                default:
                    return "Yellow";
            }
        }

        math::vector2d location_for(saucer_type type) {
            switch (type) {
                case saucer_type::blue:
                    return {30.f, 30.f};
                case saucer_type::green:
                    return {994.f, 30.f};
                case saucer_type::beige:
                    return {994.f, 738.f};
                case saucer_type::yellow:
                default:
                    return {30.f, 738.f};
            }
        }

        graphics::image image_for(saucer_type type) {
            scalar scale = 0.3;
            std::string prefix{"resources/ship"};
            std::string fn = prefix + color_for(type) + "_manned.png";
            return graphics::image{fn, scale};
        }
    } // namespace

    saucer::saucer(saucer_type type, play::stage &s)
            : play::free_roaming_actor{location_for(type)},
              drawable_{*this, image_for(type)},
              velocity_{20},
              type_{type},
              huntingIndex{-1},
              stage(s) {
        transferState(new UfoWanderState());
    }

    //! normalize fails on a vector with a length of 0, we still want a force that goes in one direction to have effect
    math::vector2d saucer::normalizeForce(math::vector2d force) const {
        if (force.x() != 0 && force.y() != 0) {
            return math::normalize(force);
        } else {
            return {static_cast<float>(force.x() > 0 ? 1 : 0), static_cast<float>(force.y() > 0 ? 1 : 0)};
        }
    }

    template <typename T>
    int saucer::findInStage() {
        for (std::size_t ix{}; ix < num_perceived_actors(); ++ix) {
            auto &other = perceived_actor(ix);
            if (auto h = dynamic_cast<T *>(&other); h) {
                //! check if human, check if dead or safe
                if(auto k = dynamic_cast<human *>(&other); k && (k->isDead() || k->isSafe())) {
                    continue;
                }
                //! check if tank, check if brokenDown
                if(auto k = dynamic_cast<tank *>(&other); k && k->brokenDown) {
                    continue;
                }
                int newIndex = 0;
                //! search for the perceived item
                for (auto it = stage.begin(); it != stage.end(); it++) {
                    auto &o = *it;
                    if (h == dynamic_cast<T *>(&o)) {
                        huntingIndex = newIndex;
                        return newIndex;
                    }
                    newIndex++;
                }
            }
        }
        //! can't find the perceived item in stage
        return -1;
    }

    void saucer::act(delta_time dt) {
        math::vector2d screenVector{0, 0};

        t_since_move_ += dt;

        if (to_seconds(t_since_move_) >= 1) {
            randomVector = {random_scalar(-1, 1), random_scalar(-1, 1)};
            t_since_move_ = from_seconds(0);
        }

        if (collidesWithScreen()) {
            //! calculate force towards the center of the screen
            screenVector += {1024 / 2 - location().x(), 768 / 2 - location().y()};
            //! normalize the vector
            screenVector = normalizeForce(screenVector);
        }

        executeState(dt, *this, screenVector);

//        //! get the first human from perceived actors and set the huntingIndex to the index of human
//        for (std::size_t ix{}; ix < num_perceived_actors(); ++ix) {
//            auto &other = perceived_actor(ix);
//            if (auto h = dynamic_cast<human *>(&other); h && huntingIndex == -1) {
//                int newIndex = 0;
//                for (auto it = stage.begin(); it != stage.end(); it++) {
//                    auto &o = *it;
//                    if (h == dynamic_cast<human *>(&o)) {
//                        huntingIndex = newIndex;
//                    }
//                    newIndex++;
//                }
//            } else if (auto t = dynamic_cast<tank *>(&other); t) {
//                if (huntingIndex != -1) {
//                    auto it = stage.begin() + huntingIndex;
//                    auto &t2 = *it;
//                    //! check if current target is a tank if not the perceived tank gets priority
//                    if (!dynamic_cast<tank *>(&t2)) {
//                        int newIndex = 0;
//                        for (auto it = stage.begin(); it != stage.end(); it++) {
//                            auto &o = *it;
//                            if (t == dynamic_cast<tank *>(&o)) {
//                                huntingIndex = newIndex;
//                            }
//                            newIndex++;
//                        }
//                    }
//                } else {
//                    int newIndex = 0;
//                    for (auto it = stage.begin(); it != stage.end(); it++) {
//                        auto &o = *it;
//                        if (t == dynamic_cast<tank *>(&o)) {
//                            huntingIndex = newIndex;
//                        }
//                        newIndex++;
//                    }
//                }
//            }
//        }
//
//        if (huntingIndex != -1) {
//            auto it = stage.begin() + huntingIndex;
//            auto &o = *it;
//
//            if (auto h = dynamic_cast<human *>(&o); h && !h->isDead() && !h->isSafe()) {
//                //! predict human location
//                auto humanAhead = h->location() + velocity_ * heading() * to_seconds(dt);
//                //! calculate vector from saucer to the human
//                huntingVector = {humanAhead.x() - location().x(), humanAhead.y() - location().y()};
//                //! normalize the vector
//                huntingVector = normalizeForce(huntingVector);
//            } else if (auto t = dynamic_cast<tank *>(&o); t && t->tankHP >= 0) {
//                //! predict tank location
//                auto tankAhead = t->location() + velocity_ * heading() * to_seconds(dt);
//                //! calculate vector from saucer to the tank
//                huntingVector = {tankAhead.x() - location().x(), tankAhead.y() - location().y()};
//                //! normalize the vector
//                huntingVector = normalizeForce(huntingVector);
//            } else {
//                //! if the human is either safe or dead set huntingIndex to -1
//                huntingIndex = -1;
//            }
//        }
//
//        math::vector2d finalVector = screenVector + huntingVector;
//        finalVector += (finalVector == math::vector2d{0, 0} ? randomVector : math::vector2d{0, 0});
//        finalVector = normalizeForce(finalVector);
//        heading(finalVector);

        for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
            auto &other = colliding_actor(ix);
            if (auto h = dynamic_cast<human *>(&other); h && huntingIndex != -1) {
                auto it = stage.begin() + huntingIndex;
                auto &o = *it;
                h->setIsDead(true);
                huntingIndex = -1;
            }
            if (auto h = dynamic_cast<tank *>(&other); h) {
                if (h->hasEMP_) {
                    transferState(new FrozenState());
                } else if (h->hasShield_) {
                    h->tankHP -= 20;
                } else {
                    h->tankHP -= 50;
                }
                if (h->tankHP <= 0) {
                    huntingIndex = -1;
                }
            }
        }

        location(location() + velocity_ * heading() * to_seconds(dt));

    }
} // namespace kmint::ufo
