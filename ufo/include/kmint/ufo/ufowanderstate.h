//
// Created by osboxes on 3/22/20.
//

#ifndef UFO_UFOWANDERSTATE_H
#define UFO_UFOWANDERSTATE_H

#include "kmint/ufo/hunthumanstate.h"
#include "kmint/ufo/hunttankstate.h"

namespace kmint::ufo {
    class UfoWanderState : public UfoBaseState {
        void executeState(delta_time dt, saucer &actor, math::vector2d screenVector) override {
            math::vector2d finalVector = screenVector;
            finalVector += (finalVector == math::vector2d{0, 0} ? actor.randomVector : math::vector2d{0, 0});
            finalVector = actor.normalizeForce(finalVector);
            actor.heading(finalVector);
            bool sawTank = false;
            bool sawHuman = false;
            for (std::size_t ix{}; ix < actor.num_perceived_actors(); ++ix) {
                auto &other = actor.perceived_actor(ix);
                if (auto h = dynamic_cast<human *>(&other)) {
                    sawHuman = true;
                }

                if (auto h = dynamic_cast<tank *>(&other); h) {
                    sawTank = true;
                }

                if (sawHuman && !sawTank) {
                    actor.transferState(new HuntHumanState());
                    return;
                } else if (sawTank) {
                    actor.transferState(new HuntTankState());
                    return;
                }
            }
            actor.setLocation((actor.location() + actor.v() * actor.heading() * to_seconds(dt)));
        }
    };
}

#endif //UFO_UFOWANDERSTATE_H
