#ifndef KMINT_UFO_SAUCER_HPP
#define KMINT_UFO_SAUCER_HPP

#include "kmint/math/vector2d.hpp"
#include "kmint/play.hpp"
#include "kmint/ufo/ufostatemanager.h"
#include "kmint/ufo/human.hpp"

namespace kmint::ufo {
    enum class saucer_type {
        blue, green, beige, yellow
    };

    class saucer : public kmint::play::free_roaming_actor, public UfoStateManager {
    public:
        saucer(saucer_type type, play::stage &s);
        math::vector2d normalizeForce(math::vector2d force) const;
        saucer_type type() const noexcept { return type_; }

        ui::drawable const &drawable() const override { return drawable_; }

        ui::drawable &getMutableDrawable() { return drawable_; }

        void act(delta_time dt) override;

        // participates in collisions
        bool incorporeal() const override { return false; }

        scalar collision_range() const override { return 32.0; }

        int v() const { return velocity_; }
        void setV(int v) { velocity_ = v; }

        void setLocation(math::vector2d loc) { location(loc); }

        template <typename T>
        int findInStage();

        bool perceptive() const override { return true; }
        scalar perception_range() const override { return 150.f; }

        math::vector2d randomVector{0, 0};
        play::stage& stage;
        int huntingIndex;
    private:
        play::image_drawable drawable_;
        saucer_type type_;
        int velocity_;
        delta_time t_since_move_{};
    };

} // namespace kmint::ufo
#endif
