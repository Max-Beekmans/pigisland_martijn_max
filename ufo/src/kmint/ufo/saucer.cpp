#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/random.hpp"
#include <string>
#include <iostream>

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
	  huntingIndex_{-1},
	  stage_(s) {}

void saucer::act(delta_time dt) {
    math::vector2d screenVector{0,0};
    math::vector2d huntingVector{0,0};

    t_since_move_ += dt;

    if(to_seconds(t_since_move_) >= 1) {
        randomVector_ = {random_scalar(-1, 1), random_scalar(-1, 1)};
        t_since_move_ = from_seconds(0);
    }

    if(collidesWithScreen()) {
        //! calculte force towards the center of the screen
        screenVector += { 1024/2 - location().x(), 768/2 - location().y() };
        //! normalize the vector
        screenVector = normalizeForce(screenVector);
    }

    //! get the first human from perceived actors and set the huntingIndex to the index of human
    for (std::size_t ix{}; ix < num_perceived_actors(); ++ix) {
        auto &other = perceived_actor(ix);
        if (auto h = dynamic_cast<human*>(&other); h && huntingIndex_ == -1) {
            int newIndex = 0;
            for(auto it = stage_.begin(); it != stage_.end(); it++) {
                auto &o = *it;
                if(h == dynamic_cast<human*>(&o)) {
                    huntingIndex_ = newIndex;
                }
                newIndex++;
            }
        } else if(auto t = dynamic_cast<tank*>(&other); t) {
            if(huntingIndex_ != -1) {
                auto it = stage_.begin()+huntingIndex_;
                auto &t2 = *it;
                //! check if current target is a tank if not the perceived tank gets priority
                if(!dynamic_cast<tank*>(&t2)) {
                    int newIndex = 0;
                    for(auto it = stage_.begin(); it != stage_.end(); it++) {
                        auto &o = *it;
                        if(t == dynamic_cast<tank*>(&o)) {
                            huntingIndex_ = newIndex;
                        }
                        newIndex++;
                    }
                }
            } else {
                int newIndex = 0;
                for(auto it = stage_.begin(); it != stage_.end(); it++) {
                    auto &o = *it;
                    if(t == dynamic_cast<tank*>(&o)) {
                        huntingIndex_ = newIndex;
                    }
                    newIndex++;
                }
            }
        }
    }

    if(huntingIndex_ != -1) {
        auto it = stage_.begin()+huntingIndex_;
        auto &o = *it;

        if(auto h = dynamic_cast<human*>(&o); h && !h->isDead() && !h->isSafe()) {
            //! predict human location
            auto humanAhead = h->location() + velocity_ * heading() * to_seconds(dt);
            //! calculate vector from saucer to the human
            huntingVector = { humanAhead.x() - location().x(), humanAhead.y() - location().y()};
            //! normalize the vector
            huntingVector = normalizeForce(huntingVector);
        } else if(auto t = dynamic_cast<tank*>(&o); t && t->tankHP > 0) {
            //! predict tank location
            auto tankAhead = t->location() + velocity_ * heading() * to_seconds(dt);
            //! calculate vector from saucer to the tank
            huntingVector = { tankAhead.x() - location().x(), tankAhead.y() - location().y()};
            //! normalize the vector
            huntingVector = normalizeForce(huntingVector);
        } else {
            //! if the human is either safe or dead set huntingIndex to -1
            huntingIndex_ = -1;
        }
    }

    math::vector2d finalVector = screenVector + huntingVector;
    finalVector += (finalVector == math::vector2d{0,0} ? randomVector_ : math::vector2d{0,0} );
    finalVector = normalizeForce(finalVector);
    heading(finalVector);

    for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
        auto &other = colliding_actor(ix);
        if (auto h = dynamic_cast<human *>(&other); h && huntingIndex_ != -1) {
            auto it = stage_.begin()+huntingIndex_;
            auto &o = *it;
            h->setIsDead(true);
        }
    }

  location(location() + velocity_ * heading() * to_seconds(dt));

}

//! normalize fails on a vector with a length of 0, we still want a force that goes in one direction to have effect
    math::vector2d saucer::normalizeForce(math::vector2d force) const {
        if(force.x() != 0 && force.y() != 0) {
            return math::normalize(force);
        } else {
            return {static_cast<float>(force.x() > 0 ? 1 : 0), static_cast<float>(force.y() > 0 ? 1 : 0) };
        }
    }

} // namespace kmint::ufo
