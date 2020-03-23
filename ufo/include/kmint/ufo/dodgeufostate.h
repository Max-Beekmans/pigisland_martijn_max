//
// Created by Max on 3/22/20.
//

#ifndef UFO_DODGEUFOSTATE_H
#define UFO_DODGEUFOSTATE_H

namespace kmint::ufo {
    class DodgeUfoState : public TankBaseState {
    public:
        void executeState(tank &actor) override {
            auto& imageDrawable = dynamic_cast<play::image_drawable &>(actor.getMutableDrawable());
            imageDrawable.set_tint(graphics::color(160, 160, 100));
            for (std::size_t ix{}; ix < actor.num_perceived_actors(); ++ix) {
                auto &other = actor.perceived_actor(ix);
                if (dynamic_cast<saucer *>(&other)) {
                    //! hCost of the largest successor
                    std::pair<float, float> maxFloatPair = {0.0, 0.0};
                    for (auto &edge : actor.node()) {
                        float h = calculate_heuristic(Heuristic::MANHATTAN,
                                edge.to().location().x(),
                                edge.to().location().y(),
                                other.location().x(),
                                other.location().y());
                        if (maxFloatPair.second < h) {
                            maxFloatPair.first = edge.to().node_id();
                            maxFloatPair.second = h;
                        }
                    }
                    //! Go to the successor with the highest hCost
                    //! (Furthest node away from ufo)
                    actor.node(actor.graph()[maxFloatPair.first]);
                    imageDrawable.remove_tint();
                    actor.previousState();
                    return;
                }
            }
            actor.previousState();
        }
    };
}

#endif //UFO_DODGEUFOSTATE_H
