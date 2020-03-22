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
        void executeState(tank &actor) override {
            if (path_.isEmpty() && !path_.reachedEnd()) {
                path_ = actor.get_path_to_shield();
            } else if(path_.reachedEnd()) {
                actor.hasShield_ = true;
                path_.deletePath();
                //TODO make it choose
                actor.previousState();
                return;
            }
            actor.node(actor.graph()[path_.popFront()->getNode()->node_id()]);
        }

    private:
        PathWrapper path_;
    };
}

#endif //UFO_TANKTRAVELTOSHIELDSTATE_H
