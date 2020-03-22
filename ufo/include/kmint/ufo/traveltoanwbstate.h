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
                path_.deletePath();
                actor.transferState(new TankWanderState());
            }
        }
    private:
        PathWrapper path_;
    };
}

#endif //UFO_TRAVELTOANWB_H
