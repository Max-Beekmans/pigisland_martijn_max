//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKTRAVELTOSHIELDSTATE_H
#define UFO_TANKTRAVELTOSHIELDSTATE_H

#include "kmint/ufo/tankbasestate.h"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo {
    class TravelToShieldState : public TankBaseState {
    public:
        void executeState(tank &actor) override {
            auto& imageDrawable = dynamic_cast<play::image_drawable &>(actor.getMutableDrawable());
            imageDrawable.set_tint(graphics::color(255, 0, 255));
            actor.shieldCount++;
            actor.previousChoice = 1;

            if (path_.isEmpty() && !path_.reachedEnd()) {
                path_ = actor.get_path_to_shield();
            } else if(path_.reachedEnd()) {
                actor.hasShield_ = true;
                float sum = actor.travelToEMPChance + actor.travelToShieldChance + actor.dodgeUfoChance;
                actor.travelToShieldChance += ((((100 - actor.tankHP)+15)/actor.shieldCount)/sum);
                path_.deletePath();
                //TODO make it choose
                imageDrawable.remove_tint();
                actor.previousState();
                return;
            }
            actor.node(actor.graph()[path_.popFront()->getNode()->node_id()]);
        }

    private:
        PathWrapper path_;
    };
}

#endif //UFO_TANKTRAVELTOSHIELDSTATE_H
