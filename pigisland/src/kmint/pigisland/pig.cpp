#include "kmint/pigisland/pig.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"

namespace kmint {
namespace pigisland {


pig::pig(math::vector2d location)
  : play::free_roaming_actor{location},
    drawable_{*this, pig_image()} {
    for (auto i = begin_perceived(); i != end_perceived(); ++i) {
        auto const &a = *i;
    }
}


} // namespace pigisland

} // namespace kmint
