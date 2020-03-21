//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKSTATEMANAGER_H
#define UFO_TANKSTATEMANAGER_H

#include <stack>
#include <vector>
#include <kmint/play/actor.hpp>
#include "tankbasestate.h"

namespace kmint::ufo {

    class tank;

    class TankStateManager {
    public:
        TankStateManager();
        ~TankStateManager();
        void transferState(TankBaseState* state);
        void previousState();
        void executeState(delta_time dt, tank& tank);
    private:
        std::stack<TankBaseState*, std::vector<TankBaseState*>> stateStack_{};
    };
}

#endif //UFO_TANKSTATEMANAGER_H
