//
// Created by osboxes on 3/23/20.
//

#ifndef UFO_UFOFORZENSTATE_H
#define UFO_UFOFORZENSTATE_H

namespace kmint::ufo {
    class FrozenState : public UfoBaseState {
    public:
        void executeState(delta_time dt, saucer &actor, math::vector2d screenVector) override {
            if(turns < 0) {
                velocity_ = actor.v();
                actor.setV(0);
                turns = 1;
            }
            t_since_move_ += dt;
            if (to_seconds(t_since_move_) >= 1) {
                turns++;
                if(turns > 20) {
                    actor.setV(velocity_);
                    actor.reset();
                    actor.transferState(new UfoWanderState());
                    return;
                }
            }
        }
    private:
        int turns = -1;
        int velocity_ = -1;
    };
}

#endif //UFO_UFOFORZENSTATE_H
