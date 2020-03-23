//
// Created by Max on 3/22/20.
//

#ifndef UFO_HUNTTANKSTATE_H
#define UFO_HUNTTANKSTATE_H

namespace kmint::ufo {
    class HuntTankState : public UfoBaseState {
        void executeState(delta_time dt, saucer &actor, math::vector2d screenVector) override {
            math::vector2d huntingVector{0, 0};
            int huntingIndex = actor.findInStage<tank>();
            if (huntingIndex < 0) {
                actor.previousState();
                return;
            }
            auto it = actor.stage.begin() + huntingIndex;
            auto &o = *it;
            auto humanAhead = dynamic_cast<tank *>(&o)->location() + actor.v() * actor.heading() * to_seconds(dt);
            huntingVector = {humanAhead.x() - actor.location().x(), humanAhead.y() - actor.location().y()};
            huntingVector = actor.normalizeForce(huntingVector);
            math::vector2d finalVector = screenVector + huntingVector;
            finalVector += (finalVector == math::vector2d{0, 0} ? actor.randomVector : math::vector2d{0, 0});
            finalVector = actor.normalizeForce(finalVector);
            actor.heading(finalVector);
        }
    };
}

#endif //UFO_HUNTTANKSTATE_H