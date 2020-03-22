#include <iostream>
#include <utility>
#include "kmint/ufo/tank.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/traveltoanwbstate.h"

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
               std::vector<size_t> shields, std::vector<size_t> grenades, PathWrapper &andrePath)
            : play::map_bound_actor{initial_node}, TankStateManager(),
              graph_{g},
              drawable_{*this, graphics::image{tank_image(t)}},
              type_{t},
              andrePath_(andrePath),
              shields_(std::move(shields)),
              grenades_(std::move(grenades)) {
        transferState(new TankWanderState());
    }

    PathWrapper tank::get_path_to_shield() {
        std::pair<float, float> minPair{MAXFLOAT, MAXFLOAT};
        for (auto s : shields_) {
            float h = calculate_heuristic(MANHATTAN, node(), graph_[s]);
            if (h < minPair.second) {
                minPair = {s, h};
            }
        }
        return tag_shortest_path_astar(MANHATTAN, node(), graph_[minPair.first], graph_);
    }

    PathWrapper tank::get_path_to_emp() {
        std::pair<float, float> minPair{MAXFLOAT, MAXFLOAT};
        for (auto s : grenades_) {
            float h = calculate_heuristic(MANHATTAN, node(), graph_[s]);
            if (h < minPair.second) {
                minPair = {s, h};
            }
        }
        return tag_shortest_path_astar(MANHATTAN, node(), graph_[minPair.first], graph_);
    }

    PathWrapper tank::get_path_to_andre() {
        return tag_shortest_path_astar(MANHATTAN, node(), graph_[andrePath_.popFront()->getNode()->node_id()],
                                       graph_);
    }

    void tank::act(delta_time dt) {
        t_since_move_ += dt;
        if (to_seconds(t_since_move_) >= 1) {
            executeState(*this);
            t_since_move_ = from_seconds(0);
        }
        if (tankHP < 0) {
            transferState(new TravelToANWBState());
        }
    }
} // namespace kmint::ufo
