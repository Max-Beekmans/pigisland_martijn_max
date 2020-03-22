#include "kmint/ufo/tank.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>
#include <kmint/ufo/saucer.hpp>
#include <utility>
#include "kmint/ufo/tankwanderstate.h"

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

    tank::tank(map::map_graph &g, map::map_node &initial_node, tank_type t,
               std::vector<size_t> shields, std::vector<size_t> grenades)
            : play::map_bound_actor{initial_node}, TankStateManager(), type_{t}, graph_{g},
              drawable_{*this, graphics::image{tank_image(t)}},
              path_(nullptr), shields_(std::move(shields)), grenades_(std::move(grenades)) {
        transferState(new TankWanderState());
    }

    PathWrapper tank::get_path_to_shield() {
        std::pair<float, float> minPair{MAXFLOAT, MAXFLOAT};
        for (auto s : shields_) {
            float h = calculate_heuristic(Heuristic::MANHATTAN, node(), graph_[s]);
            if (h < minPair.second) {
                minPair = {s, h};
            }
        }
        return tag_shortest_path_astar(ufo::MANHATTAN, node(), graph_[minPair.first], graph_);;
    }

    PathWrapper tank::get_path_to_emp() {
        std::pair<float, float> minPair{MAXFLOAT, MAXFLOAT};
        for (auto s : grenades_) {
            float h = calculate_heuristic(Heuristic::MANHATTAN, node(), graph_[s]);
            if (h < minPair.second) {
                minPair = {s, h};
            }
        }
        return tag_shortest_path_astar(ufo::MANHATTAN, node(), graph_[minPair.first], graph_);;
    }

    void tank::act(delta_time dt) {
        t_since_move_ += dt;
        if (to_seconds(t_since_move_) >= 1) {
            executeState(*this);
            t_since_move_ = from_seconds(0);
        }
//	if (to_seconds(t_since_move_) >= 1) {
//	    if(path_ == nullptr || path_->reachedEnd()) {
//            auto &dest = ufo::find_node_of_kind(graph_, '1');
//            path_ = ufo::tag_shortest_path_astar(ufo::EUCLIDEAN, node(), dest, graph_);
//	    }
//		// pick random edge
//		//int next_index = random_int(0, node().num_edges());
//		//this->node(node()[next_index].to());
//		this->node(graph_[path_->popFront()->getNode()->node_id()]);
//		t_since_move_ = from_seconds(0);
//	}
//	// laat ook zien wat hij ziet
//	for (auto i = begin_perceived(); i != end_perceived(); ++i) {
//		auto const& a = *i;
//		//std::cout << "Saw something at " << a.location().x() << ", "
//		//	<< a.location().y() << "\n";
//	}
    }
} // namespace kmint::ufo
