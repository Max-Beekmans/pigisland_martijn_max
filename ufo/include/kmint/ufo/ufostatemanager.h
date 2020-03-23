//
// Created by Max on 3/22/20.
//

#ifndef UFO_UFOSTATEMANAGER_H
#define UFO_UFOSTATEMANAGER_H

#include <stack>
#include <vector>
#include <kmint/primitives.hpp>
#include <kmint/math/vector2d.hpp>
#include "kmint/ufo/ufobasestate.h"

namespace kmint::ufo {

    class saucer;

    class UfoStateManager {
    public:
        UfoStateManager();
        ~UfoStateManager();
        void transferState(UfoBaseState *state);
        void previousState();
        void reset();
        void executeState(delta_time dt, saucer &actor, math::vector2d screenVector);

    private:
        std::stack<UfoBaseState *, std::vector<UfoBaseState *>> stateStack_{};
    };
}

#endif //UFO_UFOSTATEMANAGER_H
