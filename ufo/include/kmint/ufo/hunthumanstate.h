//
// Created by Max on 3/22/20.
//

#ifndef UFO_HUNTHUMANSTATE_H
#define UFO_HUNTHUMANSTATE_H

namespace kmint::ufo {
    class HuntHumanState : public UfoBaseState {
        void executeState(delta_time dt, saucer &actor, math::vector2d screenVector) override {
            for (std::size_t ix{}; ix < actor.num_perceived_actors(); ++ix) {
                auto &other = actor.perceived_actor(ix);
                if (auto h = dynamic_cast<tank *>(&other); h && !h->brokenDown) {
                    actor.previousState();
                    return;
                }
            }
            math::vector2d huntingVector{0, 0};
            int huntingIndex = actor.findInStage<human>();
            if (huntingIndex < 0) {
                actor.previousState();
                return;
            }
            auto it = actor.stage.begin() + huntingIndex;
            auto &o = *it;
            auto humanAhead = dynamic_cast<human *>(&o)->location() + actor.v() * actor.heading() * to_seconds(dt);
            huntingVector = {humanAhead.x() - actor.location().x(), humanAhead.y() - actor.location().y()};
            huntingVector = actor.normalizeForce(huntingVector);
            math::vector2d finalVector = screenVector + huntingVector;
            finalVector += (finalVector == math::vector2d{0, 0} ? actor.randomVector : math::vector2d{0, 0});
            finalVector = actor.normalizeForce(finalVector);
            actor.heading(finalVector);
        }
    };
}

#endif //UFO_HUNTHUMANSTATE_H
