#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include "kmint/play.hpp"
#include <tuple>
#include <vector>

namespace kmint::ufo {

class human : public kmint::play::free_roaming_actor {
public:
  human();
  const ui::drawable& drawable() const override { return drawable_; }
  void act(delta_time dt) override;
  // participates in collisions
  bool incorporeal() const override { return false; }
  scalar collision_range() const override { return 16.0; }
  bool perceptive() const override { return true; }
  scalar perception_range() const override { return 100.f; }
  math::vector2d collisionWithBuildings() const;
private:
  play::image_drawable drawable_;
  int velocity_;
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
