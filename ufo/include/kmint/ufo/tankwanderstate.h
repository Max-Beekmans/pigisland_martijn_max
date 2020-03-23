//
// Created by osboxes on 3/21/20.
//

#ifndef UFO_TANKWANDERSTATE_H
#define UFO_TANKWANDERSTATE_H

#include <kmint/random.hpp>
#include "kmint/ufo/tankbasestate.h"
#include "kmint/ufo/traveltoshieldstate.h"
#include "kmint/ufo/traveltogrenadestate.h"
#include "kmint/ufo/dodgeufostate.h"
#include "kmint/ufo/tank.hpp"

namespace kmint::ufo {
    class TankWanderState : public TankBaseState {
    public:
        void executeState(tank &actor) override {
            auto& imageDrawable = dynamic_cast<play::image_drawable &>(actor.getMutableDrawable());
            imageDrawable.set_tint(graphics::color(0, 0, 255, 0));
            int next_index = random_int(0, actor.node().num_edges());
            actor.node(actor.node()[next_index].to());
            std::cout << "Tank is wandering" << std::endl;
            for (std::size_t ix{}; ix < actor.num_perceived_actors(); ++ix) {
                auto &other = actor.perceived_actor(ix);
                if (dynamic_cast<saucer *>(&other)) {
                    imageDrawable.remove_tint();
                    int nextAction = random_int(0, 100);
                    if (nextAction < 33) {
                        std::cout << "Tank will go to grenade " << std::endl;
                        actor.transferState(new TravelToGrenadeState());
                        return;
                    } else if(nextAction > 33 && nextAction < 66) {
                        std::cout << "Tank will go to shield " << std::endl;
                        actor.transferState(new TravelToShieldState());
                        return;
                    } else {
                        std::cout << "Tank will dodge this ufo" << std::endl;
                        actor.transferState(new DodgeUfoState());
                        return;
                    }
                }
            }
        }
    };
}

#endif //UFO_TANKWANDERSTATE_H
