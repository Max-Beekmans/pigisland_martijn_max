//
// Created by Max on 3/22/20.
//

#include "kmint/ufo/ufostatemanager.h"

namespace kmint::ufo {
    UfoStateManager::UfoStateManager() = default;

    UfoStateManager::~UfoStateManager() {
        while(!stateStack_.empty()) {
            stateStack_.pop();
        }
    }

    void UfoStateManager::transferState(kmint::ufo::UfoBaseState *state) {
        stateStack_.push(state);
    }

    void UfoStateManager::previousState() {
        stateStack_.pop();
    }

    void UfoStateManager::reset() {
        while(!stateStack_.empty()) {
            stateStack_.pop();
        }
    }

    void UfoStateManager::executeState(delta_time dt, kmint::ufo::saucer &actor) {
        stateStack_.top()->executeState(dt, actor);
    }
}
