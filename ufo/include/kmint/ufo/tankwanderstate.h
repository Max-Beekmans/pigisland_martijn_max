//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKWANDERSTATE_H
#define UFO_TANKWANDERSTATE_H

#include "kmint/ufo/tankbasestate.h"
#include "kmint/ufo/traveltoshieldstate.h"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo {
    class TankWanderState : public TankBaseState {
    public:
        void executeState(delta_time dt, tank& actor) {
            t_since_move_ += dt;
            if(to_seconds(t_since_move_) >= 1) {
                int next_index = random_int(0, actor.node().num_edges());
                actor.node(actor.node()[next_index].to());
                t_since_move_ = from_seconds(0);
                for (std::size_t ix{}; ix < actor.num_perceived_actors(); ++ix) {
                    auto &other = actor.perceived_actor(ix);
                    if (dynamic_cast<saucer*>(&other)) {
                        actor.transferState(new TravelToShieldState());
                    }
                }
            }
        }
    };
}

#endif //UFO_TANKWANDERSTATE_H
