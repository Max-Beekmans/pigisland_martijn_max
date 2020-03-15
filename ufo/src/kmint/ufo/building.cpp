#include "kmint/ufo/building.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {

    building::building(math::vector2d location, math::size size)
            : free_roaming_actor{ location }, drawable_{*this, graphics::image{"", 0}}, size_{size} {}

    math::rectangle building::collision_box() const {
        return {{location().x() - size_.width()/2, location().y() - size_.height()/2}, size_};
    }
} // namespace kmint::ufo
