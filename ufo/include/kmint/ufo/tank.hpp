#ifndef KMINT_UFO_TANK_HPP
#define KMINT_UFO_TANK_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "node_algorithm.hpp"
#include "kmint/ufo/tankstatemanager.h"

namespace kmint::ufo {

    enum class tank_type {
        red, green
    };

    class tank : public play::map_bound_actor, public TankStateManager {
    public:
        tank(map::map_graph &g,
             map::map_node &initial_node,
             tank_type t,
             std::vector<size_t> shields,
             std::vector<size_t> grenades,
             PathWrapper &andrePath);

        tank &operator=(tank &&) = default;

        // wordt elke game tick aangeroepen
        void act(delta_time dt) override;

        ui::drawable const &drawable() const override { return drawable_; }

        ui::drawable &getMutableDrawable() { return drawable_; }

        // als incorporeal false is, doet de actor mee aan collision detection
        bool incorporeal() const override { return false; }

        // geeft de lengte van een zijde van de collision box van deze actor terug.
        // Belangrijk voor collision detection
        scalar collision_range() const override { return 16.0; }

        // geeft aan dat de tank andere actors kan zien
        bool perceptive() const override { return true; }

        // geeft het bereik aan waarbinnen een tank
        // andere actors kan waarnemen.
        scalar perception_range() const override { return 200.f; }

        tank_type type() const { return type_; }

        map::map_graph &graph() const { return graph_; }

        /// Get path to nearest shield
        /// \return PathWrapper obj
        PathWrapper get_path_to_shield();

        /// Get path to nearest EMP grenade
        /// \return PathWrapper obj
        PathWrapper get_path_to_emp();

        /// Get path to andre
        /// \return PathWrapper obj
        PathWrapper get_path_to_andre();

        bool hasEMP_ = true;
        bool hasShield_ = false;
        bool brokenDown = false;
        int tankHP = 100;
        int previousHP = 100;
        int previousChoice = -1;
        float dodgeUfoChance = 33.33;
        float dodgeUfoFitness = 1;
        float travelToShieldChance = 33.33;
        float travelToShieldFitness = 1;
        float travelToEMPChance = 33.33;
        float travelToEMPFitness = 1;
    private:
        map::map_graph &graph_;
        play::image_drawable drawable_;
        delta_time t_since_move_{};
        tank_type type_;
        PathWrapper &andrePath_;
        std::vector<size_t> shields_;
        std::vector<size_t> grenades_;
    };
} // namespace kmint

#endif /* KMINT_UFO_TANK_HPP */
