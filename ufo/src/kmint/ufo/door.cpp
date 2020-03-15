#include "kmint/ufo/door.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {

    door::door(math::vector2d location)
            : free_roaming_actor{ location }, drawable_{*this, graphics::image{"", 0}} {}


} // namespace kmint::ufo
