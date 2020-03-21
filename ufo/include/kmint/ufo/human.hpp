#ifndef KMINT_UFO_HUMAN_HPP
#define KMINT_UFO_HUMAN_HPP

#include "kmint/play.hpp"
#include "dna_entity.hpp"
#include <tuple>
#include <vector>

namespace kmint::ufo {

class human : public kmint::play::free_roaming_actor, public dna_entity {
public:
  human(int humanCounter);
  const ui::drawable& drawable() const override { return drawable_; }
  bool must_draw() const override { return !isDead_ && !isSafe_; }
  void act(delta_time dt) override;
  // participates in collisions
  bool incorporeal() const override { return false; }
  scalar collision_range() const override { return 32.0; }
  bool perceptive() const override { return true; }
  scalar perception_range() const override { return 100.f; }
  math::vector2d collisionWithBuildings() const;
  bool isDead() const;
  void setIsDead(bool isDead);
  bool isSafe() const;
  void setIsSafe(bool isSafe);
  int getFitness() const;
  void setFitness(int fitness);
  void setRandomLocation();
private:
  play::image_drawable drawable_;
  int velocity_;
  bool isDead_;
  bool isSafe_;
  int fitness_;
  int humanCounter_;
};

} // namespace kmint::ufo

#endif /* KMINT_UFO_HUMAN_HPP */
