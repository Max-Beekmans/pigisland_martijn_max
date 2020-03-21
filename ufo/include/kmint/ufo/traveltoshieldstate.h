//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKTRAVELTOSHIELDSTATE_H
#define UFO_TANKTRAVELTOSHIELDSTATE_H

#include "kmint/ufo/tankbasestate.h"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo {
    class TravelToShieldState : public TankBaseState {
    public:
        void executeState(delta_time dt, tank& actor) {
            t_since_move_ += dt;
            if(to_seconds(t_since_move_) >= 1) {

            }
        }
    private:
        PathWrapper* path_{};
    };
}

#endif //UFO_TANKTRAVELTOSHIELDSTATE_H
