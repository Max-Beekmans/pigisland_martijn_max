#include "kmint/ufo/door.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {

    door::door(math::vector2d location)
            : free_roaming_actor{ location }, drawable_{*this, graphics::image{"", 0}} {}

    void door::act(delta_time dt) {
        for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
            auto &other = colliding_actor(ix);
            if (auto h = dynamic_cast<human *>(&other); h) {
                h->setIsSafe(true);
            }
        }
    }
} // namespace kmint::ufo
