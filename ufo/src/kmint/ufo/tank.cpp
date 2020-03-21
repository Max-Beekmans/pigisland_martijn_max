#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint::ufo {

namespace {
graphics::image tank_image(tank_type t) {
  constexpr scalar scale = 0.35;
  switch (t) {
  case tank_type::red:
    return graphics::image{"resources/tank_red.png", scale};
  case tank_type::green:
    return graphics::image{"resources/tank_green.png", scale};
  }
  return graphics::image{"resources/tank_green.png", scale};
}


} // namespace

tank::tank(map::map_graph &g, map::map_node& initial_node, tank_type t)
	: play::map_bound_actor{ initial_node }, type_{t}, graph_{g},
	    drawable_{ *this, graphics::image{tank_image(t)}}, path_(nullptr) {}

void tank::act(delta_time dt) {
	t_since_move_ += dt;
	if (to_seconds(t_since_move_) >= 1) {
	    if(path_ == nullptr || path_->reachedEnd()) {
            auto &dest = ufo::find_node_of_kind(graph_, '1');
            path_ = ufo::tag_shortest_path_astar(ufo::EUCLIDEAN, node(), dest, graph_);
	    }
		// pick random edge
		//int next_index = random_int(0, node().num_edges());
		//this->node(node()[next_index].to());
		this->node(graph_[path_->popFront()->getNode()->node_id()]);
		t_since_move_ = from_seconds(0);
	}

    for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
        auto &other = colliding_actor(ix);
        if (auto h = dynamic_cast<human *>(&other); h) {
            type_ == tank_type::green ? h->setIsSafe(true) : h->setIsDead(true);
        }
    }
}

} // namespace kmint::ufo
