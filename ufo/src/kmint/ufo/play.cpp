#include "kmint/ufo/play.hpp"
#include "kmint/main.hpp" // voor de main loop
#include "kmint/play.hpp"
#include "kmint/ufo/andre.hpp"
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/resources.hpp"
#include "kmint/ufo/saucer.hpp"
#include "kmint/ufo/tank.hpp"
#include "kmint/ufo/door.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/ui.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <kmint/ufo/building.hpp>
#include <kmint/ufo/population.hpp>

namespace kmint::ufo {

int play() {
  // een app object is nodig om
  ui::app app{};

  // maak een venster aan
  ui::window window{app.create_window({1024, 768}, "kmint", 1.0)};

  // maak een podium aan
  play::stage s{ {1024, 768} };
  std::unique_ptr<population> pop = std::make_unique<kmint::ufo::population>(s);
  int humanCounter = 0;
  int roundCounter = 0;
  delta_time timer{};

  auto m = map();
  auto& graph = m.graph();

  s.build_actor<play::background>(math::size(1024, 768),
                                  graphics::image{m.background_image()});
  s.build_actor<play::map_actor>(math::vector2d{0.f, 0.f}, m.graph());

  for (std::size_t h{0}; h < 100; ++h) {
    s.build_actor<ufo::human>(humanCounter);
  }

  s.build_actor<ufo::building>(math::vector2d {576, 64}, math::size{80, 48}); //row 1, building 1
  s.build_actor<ufo::building>(math::vector2d {624, 64}, math::size{112, 62}); //row 1, building 2
  s.build_actor<ufo::building>(math::vector2d {576, 208}, math::size{96, 112}); //row 2, building 1
  s.build_actor<ufo::building>(math::vector2d {320, 512}, math::size{112, 128}); //row 3, building 1
  s.build_actor<ufo::building>(math::vector2d {432, 464}, math::size{80, 160}); //row 3, building 2
  s.build_actor<ufo::building>(math::vector2d {576, 400}, math::size{112, 96}); //row 3, building 3

  s.build_actor<ufo::door>(math::vector2d {600 , 137}); //row 1, building 1
  s.build_actor<ufo::door>(math::vector2d {688 , 119}); //row 1, building 2
  s.build_actor<ufo::door>(math::vector2d {625 , 315}); //row 2, building 1
  s.build_actor<ufo::door>(math::vector2d {401 , 634}); //row 3, building 1
  s.build_actor<ufo::door>(math::vector2d {456 , 620}); //row 3, building 2
  s.build_actor<ufo::door>(math::vector2d {641 , 491}); //row 3, building 3

  s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::red);
  s.build_actor<ufo::tank>(graph, ufo::random_node_of_kind(m, 'T'), tank_type::green);
  s.build_actor<ufo::andre>(graph, ufo::random_node_of_kind(m, 'R'));
  s.build_actor<ufo::saucer>(saucer_type::blue);
  s.build_actor<ufo::saucer>(saucer_type::green);
  s.build_actor<ufo::saucer>(saucer_type::beige);
  s.build_actor<ufo::saucer>(saucer_type::yellow);

  // Maak een event_source aan (hieruit kun je alle events halen, zoals
  // toetsaanslagen)
  ui::events::event_source event_source{};

  // main_loop stuurt alle actors aan.
  main_loop(s, window, [&](delta_time dt, loop_controls &ctl) {
    // gebruik dt om te kijken hoeveel tijd versterken is
    // sinds de vorige keer dat deze lambda werd aangeroepen
    // loop controls is een object met eigenschappen die je kunt gebruiken om de
    // main-loop aan te sturen.
    timer += dt;
    if(to_seconds(timer) >= 1) {
      roundCounter++;
      timer = from_seconds(0);
    }

    if(humanCounter >= 100 || roundCounter >= 200) {
      pop->populate();
    }

    for (ui::events::event &e : event_source) {
      // event heeft een methode handle_quit die controleert
      // of de gebruiker de applicatie wilt sluiten, en zo ja
      // de meegegeven functie (of lambda) aanroept om met het
      // bijbehorende quit_event
      //
      e.handle_quit([&ctl](ui::events::quit_event qe) { ctl.quit = true; });
      e.handle_key_up([&](auto ke) {
        switch (ke.key) {
        case ui::events::key::p:
          ctl.pause = !ctl.pause;
          break;
        case ui::events::key::r:
          ctl.render = !ctl.render;
          break;
        case ui::events::key::opening_bracket:
          ctl.time_scale /= 2.0;
          break;
        case ui::events::key::closing_bracket:
          ctl.time_scale *= 2.0;
          break;
        default:
          break;
        }
      });
    }
  });
  return 0;
}

} // namespace kmint::ufo
