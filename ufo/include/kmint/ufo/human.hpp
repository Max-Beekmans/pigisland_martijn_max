#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include "kmint/play.hpp"
#include "dna_entity.hpp"
#include <tuple>
#include <vector>

namespace kmint::ufo {

class human : public kmint::play::free_roaming_actor, public dna_entity {
public:
  human(math::vector2d location);
  const ui::drawable& drawable() const override { return drawable_; }
  bool must_draw() const override { return !isDead_ && !isSafe_; }
  void act(delta_time dt) override;
  // participates in collisions
  bool incorporeal() const override { return isDead_ || isSafe_; }
  scalar collision_range() const override { return 22.0; }
  bool perceptive() const override { return !isDead_ && !isSafe_; }
  bool perceivable() const override { return !isDead_ && !isSafe_; }
  scalar perception_range() const override { return 100.f; }
  math::vector2d normalizeForce(math::vector2d force) const;
  bool isDead() const;
  void setIsDead(bool isDead);
  bool isSafe() const;
  void setIsSafe(bool isSafe);
  int getFitness() const;
  void setFitness(int fitness);
  void setRandomLocation(math::vector2d loc) {
      location(loc);
  }
private:
  play::image_drawable drawable_;
  int velocity_;
  bool isDead_;
  bool isSafe_;
  int fitness_;
  delta_time t_since_move_{};
  math::vector2d randomVector_{0,0};
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
