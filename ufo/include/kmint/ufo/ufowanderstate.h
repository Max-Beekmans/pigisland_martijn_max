//
// Created by osboxes on 3/22/20.
//

#ifndef UFO_UFOWANDERSTATE_H
#define UFO_UFOWANDERSTATE_H

namespace kmint::ufo {
    class UfoWanderState : public UfoBaseState {
        void executeState(delta_time dt, saucer &actor) override {
            actor.setLocation(actor.location() + actor.v() * to_seconds(dt));
        }
    };
}

#endif //UFO_UFOWANDERSTATE_H
