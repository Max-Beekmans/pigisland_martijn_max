//
// Created by Max on 3/21/20.
//

#ifndef UFO_TANKBASESTATE_H
#define UFO_TANKBASESTATE_H

#include <kmint/primitives.hpp>

namespace kmint::ufo {
    class tank;

    class TankBaseState {
    public:
        virtual ~TankBaseState() = default;
        virtual void executeState(tank &actor) = 0;
    };
}

#endif //UFO_TANKBASESTATE_H
