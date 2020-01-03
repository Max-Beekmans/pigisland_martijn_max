#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <algorithm>
#include <numeric>

namespace kmint {
namespace pigisland {

map::map_node const &random_adjacent_node(map::map_node const &node) {
  int r = random_int(0, node.num_edges());
  return node[r].to();
}

map::map_node const &find_node_of_kind(map::map_graph const &graph, char kind) {
  auto i = std::find_if(graph.begin(), graph.end(), [kind](auto const &node) {
    return node.node_info().kind == kind;
  });
  return *i;
}
map::map_node &find_node_of_kind(map::map_graph &graph, char kind) {
  auto i = std::find_if(graph.begin(), graph.end(), [kind](auto &node) {
    return node.node_info().kind == kind;
  });
  return *i;
}

map::map_node const &find_random_mooring_place(map::map_graph const &graph) {
  return find_node_of_kind(graph, '1' + random_int(0, 3));
}

map::map_node const &find_shark_resting_place(map::map_graph const &graph) {
  return find_node_of_kind(graph, 'K');
}

map::map_node const &find_closest_node_to(map::map_graph const &graph,
                                          math::vector2d location) {
  auto i = std::min_element(graph.begin(), graph.end(),
                            [location](auto const &l, auto const &r) {
                              return distance2(l.location(), location) <
                                     distance2(r.location(), location);
                            });
  return *i;
}

double waiting_time(map::map_node const &node) { return node[0].weight(); }

} // namespace pigisland
} // namespace kmint
