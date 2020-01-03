#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include <array>
#include <random>
#include <algorithm>


namespace kmint {
namespace pigisland {
namespace {
constexpr const char *graph_description = R"graph(32 24 32
resources/pigisland.png
W 1 1
1 1 1
2 1 1
3 1 1
R 1 4
L 0 0
S 0 0
K 1 1

LLLLLLWW1WWWWRWWWWWWWWWWRWWWLLLL
LLLLLLLLLWWWWRWWWWWWWWWWRWWWLLLL
LLLLLLWWWWWWRRWWWWWWWWWWRRWWWWWL
WWWWWWWWWWWRRWWWWWWWWWWWWRRWWW2L
WWWWWWWWWWRRWWWWWWWWWWWWWRRRWWWW
WRRRRRRRRRRWWWWWWWWWWWWWWWRRWWWW
WWRRRRRRRRWWWWWWWWWWWWWWWRRRRWWW
WRRWWWRWWWWWWWWWWWWWWWWWWWWRRRRW
WWWWWWWWWWWWWLLLLLWWWWWWWWWWWWWW
WWWWWWWWWWWWWLLLLLLWWWWWWWWWWWWW
WWWWWWWWWWWWWLLLLLLWWWWWWWWWWWWW
WWWWWWWWWWWWWLLLLLLLWWWWWWWWWWWW
WWWWWWWWWWWWWLLLLLLLWWWWWWWWWWWW
WWWWWWWWWWWWWWLLLLLWWWWWWRRRRRWW
WWWWWWWWWWWWWWWWWWWWWWWWRRWRRRRW
WWWWWRRRWWWWWWWWWWWWWWWWRRRWWWWW
WWWWWWWWRWWWWWWWWWWWWWWWWRWWWWWW
WWLLLLLWWRWWWWWWWWWWWWWRRWWWL3WW
WWLLLLLWWRRWWWWWWWWWWWRRWWWWLWLL
WWLLLLLWWWRRWWWWWWWWWWRWWWLLLLLL
SSLLLLWWWWWRRRWWWWWWWWWWWLLLLLLL
SSLLLLWWWWWWWRRRRWWWWWWWWLLLLLLL
SSWWWWWWWWWWWWWWWWWWWWWWWLLLLLLL
KWWWWWWWWWWWWWWWWWWWWWWWWLLLLLLL
)graph";
}

map::map map() { return map::read_map(graph_description); }
graphics::image shark_image() { return graphics::image{"resources/shark.png"}; }
graphics::image pig_image() { return graphics::image{"resources/pig.png"}; }
graphics::image boat_image() { return graphics::image{"resources/boat.png"}; }
std::vector<math::vector2d> random_pig_locations(std::size_t n) {
  // from 0x2 to 13x17  = 195
  // from 13x0 to 28x9  = 135
  // from 20x9 to 32x19 = 120
  // from 6x17 to 25x24 = 133
  // sum                = 583
  std::random_device rd{};
  std::default_random_engine re{rd()};
  std::uniform_int_distribution<> id{0, 583};
  std::uniform_real_distribution<scalar> sd{0, 1};

  auto rv = [&](math::rectangle const &r) {
    return r.top_left() + math::vector2d{r.size().width() * sd(re),
                                         r.size().height() * sd(re)};
  };

  std::array rects{
      math::rectangle{math::vector2d{0.1f, 2}, math::size{13, 15}},
      math::rectangle{math::vector2d{13.f, 0.1f}, math::size{15, 9}},
      math::rectangle{math::vector2d{20, 9}, math::size{12, 10}},
      math::rectangle{math::vector2d{6, 17}, math::size{17, 6}}};
  auto to_index = [](int i) -> std::size_t {
    if (i < 195)
      return 0;
    else if (i < 330)
      return 1;
    else if (i < 450)
      return 2;
    else
      return 3;
  };

  std::vector<math::vector2d> result(n);
  std::generate_n(result.begin(), result.size(), [&] {
    auto val = id(re);
    auto index = to_index(val);
    auto rect = rects[index];
    return 32 * rv(rect);
  });
  return result;
}
} // namespace pigisland

} // namespace kmint
