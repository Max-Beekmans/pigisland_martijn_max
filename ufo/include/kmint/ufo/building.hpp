#ifndef KMINT_UFO_BUILDING_HPP
#define KMINT_UFO_BUILDING_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"


namespace kmint::ufo {

class building : public play::free_roaming_actor {
public:
    building(math::vector2d location, math::size size);
	// als incorporeal false is, doet de actor mee aan collision detection
	bool incorporeal() const override { return false; }
    ui::drawable const &drawable() const override { return drawable_; }
    math::rectangle collision_box() const override;
    bool must_draw() const override { return false; }
private:
    play::image_drawable drawable_;
	math::size size_;
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_BUILDING_HPP */
