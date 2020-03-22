//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKWANDERSTATE_H
#define UFO_TANKWANDERSTATE_H

#include "kmint/ufo/tankbasestate.h"
#include "kmint/ufo/traveltoshieldstate.h"
#include "kmint/ufo/traveltogrenadestate.h"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo {
    class TankWanderState : public TankBaseState {
    public:
        void executeState(tank &actor) {
            int next_index = random_int(0, actor.node().num_edges());
            actor.node(actor.node()[next_index].to());
            for (std::size_t ix{}; ix < actor.num_perceived_actors(); ++ix) {
                auto &other = actor.perceived_actor(ix);
                if (dynamic_cast<saucer *>(&other)) {
                    //actor.transferState(new TravelToShieldState());
                    actor.transferState(new TravelToGrenadeState());
                    return;
                }
            }
        }
    };
}

#endif //UFO_TANKWANDERSTATE_H
