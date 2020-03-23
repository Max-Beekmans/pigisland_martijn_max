//
// Created by osboxes on 3/22/20.
//

#ifndef UFO_TRAVELTOANWB_H
#define UFO_TRAVELTOANWB_H

#include "kmint/ufo/tankwanderstate.h"
#include "tankwanderstate.h"

namespace kmint::ufo {
    class TravelToANWBState : public TankBaseState {
    public:
        TravelToANWBState() = default;
        void executeState(tank &actor) override {
            auto& imageDrawable = dynamic_cast<play::image_drawable &>(actor.getMutableDrawable());
            imageDrawable.set_tint(graphics::color(0, 255, 0));
            if(path_.isEmpty() && !path_.reachedEnd()) {
                path_ = actor.get_path_to_andre();
            } else if (path_.reachedEnd()) {
                actor.tankHP = 100;
                actor.brokenDown = false;
                path_.deletePath();
                actor.reset();
                imageDrawable.remove_tint();
                actor.transferState(new TankWanderState());
                return;
            }
            actor.node(actor.graph()[path_.popFront()->getNode()->node_id()]);
        }
    private:
        PathWrapper path_;
    };
}

#endif //UFO_TRAVELTOANWB_H
