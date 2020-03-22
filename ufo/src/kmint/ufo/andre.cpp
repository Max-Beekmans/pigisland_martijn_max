#include "kmint/ufo/andre.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {

namespace {
    graphics::image andre_image() {
      constexpr scalar scale = 1.0;
      return graphics::image{"resources/andre.png"};
    }
} // namespace

andre::andre(map::map_graph& g, map::map_node& initial_node):
    play::map_bound_actor{ initial_node },
	drawable_{ *this,graphics::image{andre_image()}},
	graph_{g} {}

void andre::act(delta_time dt) {
  t_since_move_ += dt;
  if (to_seconds(t_since_move_) >= 1) {
      if(path_.isEmpty() || path_.reachedEnd()) {
          if(dest == 0 || dest > 4) {
              dest = 1;
          } else {
              dest++;
          }
          char kind;
          switch(dest) {
              case 1:
                  kind = '1';
                  break;
              case 2:
                  kind = '2';
                  break;
              case 3:
                  kind = '3';
                  break;
              case 4:
                  kind = '4';
                  break;
              default:
                  return;
          }
          auto &destNode = ufo::find_node_of_kind(graph_, kind);
          path_ = ufo::tag_shortest_path_astar(ufo::MANHATTAN, node(), destNode, graph_);
      }
      this->node(graph_[path_.popFront()->getNode()->node_id()]);
	  t_since_move_ = from_seconds(0);
  }
}

} // namespace kmint::ufo
