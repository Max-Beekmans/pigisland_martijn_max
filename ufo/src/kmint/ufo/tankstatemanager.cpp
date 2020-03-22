//
// Created by osboxes on 3/21/20.
//

#include "kmint/ufo/tankstatemanager.h"

namespace kmint::ufo {
    TankStateManager::TankStateManager() = default;

    TankStateManager::~TankStateManager() {
        while(!stateStack_.empty()) {
            stateStack_.pop();
        }
    }

    void TankStateManager::transferState(TankBaseState *state) {
        stateStack_.push(state);
    }

    void TankStateManager::previousState() {
        stateStack_.pop();
    }

    void TankStateManager::executeState(tank &stateActor) {
        stateStack_.top()->executeState(stateActor);
    }
}