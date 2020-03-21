//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKBASESTATE_H
#define UFO_TANKBASESTATE_H

#include <kmint/primitives.hpp>

namespace kmint::ufo {
    class tank;

    class TankBaseState {
    public:
        virtual void executeState(delta_time dt, tank &actor) = 0;
    protected:
        delta_time t_since_move_{};
    };
}

#endif //UFO_TANKBASESTATE_H
