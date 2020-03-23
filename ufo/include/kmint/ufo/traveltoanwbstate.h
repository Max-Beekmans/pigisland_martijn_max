//
// Created by osboxes on 3/22/20.
//

#ifndef UFO_TRAVELTOANWB_H
#define UFO_TRAVELTOANWB_H

#include "kmint/ufo/tankwanderstate.h"
#include "tankwanderstate.h"

namespace kmint::ufo {
    class TravelToANWBState : public TankBaseState {
    public:
        TravelToANWBState() = default;
        void executeState(tank &actor) override {
            if(path_.isEmpty() && !path_.reachedEnd()) {
                path_ = actor.get_path_to_andre();
            } else if (path_.reachedEnd()) {
                actor.tankHP = 100;
                actor.brokenDown = false;
                path_.deletePath();
                actor.reset();
                actor.transferState(new TankWanderState());
                return;
            }
            actor.node(actor.graph()[path_.popFront()->getNode()->node_id()]);
        }
    private:
        PathWrapper path_;
    };
}

#endif //UFO_TRAVELTOANWB_H
