#include "kmint/play/image_drawable.hpp"
#include "kmint/math/vector2d.hpp"
#include "kmint/play/actor.hpp"
#include "kmint/ui/window.hpp"
namespace kmint::play {

void image_drawable::draw(ui::frame &f) const {
  if (tinted_) {
    f.draw_image(actor_->getLocation(), image_, tint_);
  } else {
    f.draw_image(actor_->getLocation(), image_);
  }
}

} // namespace kmint::play
