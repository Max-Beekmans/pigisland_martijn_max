//
// Created by osboxes on 3/23/20.
//

#ifndef UFO_UFOFORZENSTATE_H
#define UFO_UFOFORZENSTATE_H

namespace kmint::ufo {
    class FrozenState : public UfoBaseState {
    public:
        void executeState(delta_time dt, saucer &actor, math::vector2d screenVector) override {
            auto& imageDrawable = dynamic_cast<play::image_drawable &>(actor.getMutableDrawable());
            imageDrawable.set_tint(graphics::color(255, 127, 0));
            std::cout << "Ufo is frozen" << std::endl;
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
                    imageDrawable.remove_tint();
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
