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

        void find_path_astar(map::map_graph &g, map::map_node &start_node) {
            //list of node_id, distance pairs
            std::vector<std::tuple<std::size_t, int16_t, bool>> nodes{};
            //get all nodes into list and set all distances to max
            //except start_node is set to 0
            for (auto &i : g) {
                int16_t val = std::numeric_limits<int16_t>::max();
                if(i.node_id() == start_node.node_id()) { val = 0; }
                nodes.emplace_back(i.node_id(), val, false);
            }
            //get node with shortest distance which is unvisited
            auto res = std::min_element(begin(nodes), end(nodes), [](const auto& lhs, const auto& rhs) {
                return (std::get<1>(lhs) < std::get<1>(rhs)) && !std::get<2>(lhs);
            });

        }


    } // namespace pigisland
} // namespace kmint
