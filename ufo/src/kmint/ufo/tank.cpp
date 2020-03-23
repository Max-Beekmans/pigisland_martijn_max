#include <iostream>
#include <utility>
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/human.hpp"
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
        PathWrapper pathToAndre;
        for (int i = 0; i < andrePath_.size(); ++i) {
            float tankHCost = calculate_heuristic(MANHATTAN, node(), graph_[andrePath_.at(i)->getNode()->node_id()]);
            float andreHCost = calculate_heuristic(MANHATTAN, graph_[andrePath_.front()->getNode()->node_id()], graph_[andrePath_.at(i)->getNode()->node_id()]);
            if(tankHCost < andreHCost) {
                pathToAndre = tag_shortest_path_astar(MANHATTAN, node(), graph_[andrePath_.at(i)->getNode()->node_id()], graph_);
                return pathToAndre;
            }
        }
        int firstKind = 0;
        int prevKind = graph_[andrePath_.back()->getNode()->node_id()].node_info().kind - '0';
        firstKind = prevKind;
        bool first = true;
        while(firstKind != prevKind || first) {
            first = false;
            if(prevKind == 4) {
                prevKind = 0;
            }
            PathWrapper andrePathNext = tag_shortest_path_astar(MANHATTAN,
                                                                graph_[andrePath_.back()->getNode()->node_id()],
                                                                find_node_of_kind(graph_, to_char_lit(++prevKind)),graph_);
            for (int j = 0; j < andrePath_.size(); ++j) {
                float tankHCost = calculate_heuristic(MANHATTAN, node(), graph_[andrePath_.at(j)->getNode()->node_id()]);
                float andreHCost = calculate_heuristic(MANHATTAN, graph_[andrePath_.front()->getNode()->node_id()], graph_[andrePath_.at(j)->getNode()->node_id()]);
                if(tankHCost < andreHCost) {
                    pathToAndre = tag_shortest_path_astar(MANHATTAN, node(), graph_[andrePath_.at(j)->getNode()->node_id()], graph_);
                    return pathToAndre;
                }
            }
            andrePathNext.deletePath();
            firstKind = prevKind;
        }
        std::cout << "I'm to far away from andre to catch up. D:" << std::endl;
        pathToAndre = tag_shortest_path_astar(MANHATTAN, node(), find_node_of_kind(graph_, '1'), graph_);
        return pathToAndre;
    }

    void tank::act(delta_time dt) {
        t_since_move_ += dt;
        if (to_seconds(t_since_move_) >= 1) {
            executeState(*this);
            t_since_move_ = from_seconds(0);
        }

        for (std::size_t ix{}; ix < num_colliding_actors(); ++ix) {
            auto &other = colliding_actor(ix);
            if (auto h = dynamic_cast<human *>(&other); h) {
                type_ == tank_type::green ? h->setIsSafe(true) : h->setIsDead(true);
            }
        }

        if (tankHP <= 0 && !brokenDown) {
            brokenDown = true;
            float sum = travelToEMPChance + travelToShieldChance + dodgeUfoChance;
            switch(previousChoice) {
                case 0:
                    //travelToEMPChance += (((100 - tankHP)/empCount)/sum);
                    break;
                case 1:
                    //travelToShieldChance += (((100 - tankHP)/shieldCount)/sum);
                    break;
                case 2:
                    //dodgeUfoChance += (((100 - tankHP)/dodgeCount)/sum);
                    break;
                default :
                    break;
            }
            std::cout << "Tank broke down" << std::endl;
            transferState(new TravelToANWBState());
        }
    }
} // namespace kmint::ufo
