//
// Created by osboxes on 3/22/20.
//

#ifndef UFO_TRAVELTOGRENADESTATE_H
#define UFO_TRAVELTOGRENADESTATE_H

namespace kmint::ufo {
    class TravelToGrenadeState : public TankBaseState {
    public:
        void executeState(tank &actor) override {
            if (path_.isEmpty() && !path_.reachedEnd()) {
                path_ = actor.get_path_to_emp();
            } else if (path_.reachedEnd()) {
                actor.hasEMP_ = true;
                path_.deletePath();
                actor.previousState();
                return;
            }
            actor.node(actor.graph()[path_.popFront()->getNode()->node_id()]);
        }

    private:
        PathWrapper path_;
    };
}

#endif //UFO_TRAVELTOGRENADESTATE_H
