//
// Created by Max on 3/22/20.
//

#ifndef UFO_UFOBASESTATE_H
#define UFO_UFOBASESTATE_H

namespace kmint::ufo {
    class saucer;

    class UfoBaseState {
    public:
        virtual ~UfoBaseState() = default;
        virtual void executeState(delta_time dt, saucer &actor) = 0;
    protected:
        delta_time t_since_move_{};
    };
}

#endif //UFO_UFOBASESTATE_H
