#ifndef KMINT_UFO_DOOR_HPP
#define KMINT_UFO_DOOR_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"


namespace kmint::ufo {

class door : public play::free_roaming_actor {
public:
    door(math::vector2d location);
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
	// geeft de lengte van een zijde van de collision box van deze actor terug.
	// Belangrijk voor collision detection
	scalar collision_range() const override { return 16.0; }
    ui::drawable const &drawable() const override { return drawable_; }
    bool must_draw() const override { return false; }
private:
    play::image_drawable drawable_;
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_DOOR_HPP */
