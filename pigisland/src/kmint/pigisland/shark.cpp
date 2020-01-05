#include "kmint/pigisland/shark.hpp"
#include "kmint/pigisland/node_algorithm.hpp"
#include "kmint/pigisland/resources.hpp"
#include "kmint/random.hpp"
#include <iostream>

namespace kmint {
    namespace pigisland {
        shark::shark(map::map_graph &g, map::map_node &initial_node)
                : play::map_bound_actor{initial_node}, drawable_{
                *this, graphics::image{shark_image()}
        }, graph_(g) {}

        void shark::act(delta_time dt) {
            //find path from position of shark to destination (restplace for test rn)
            //pigisland::find_path_astar(graph_, this->node(),pigisland::find_node_of_kind(graph_, 'K').location());
            pigisland::find_path_astar(this->graph_, this->node());
            t_passed_ += dt;
            if (to_seconds(t_passed_) >= 1) {
                // pick random edge
                int next_index = random_int(0, node().num_edges());
                this->node(node()[next_index].to());
                t_passed_ = from_seconds(0);
            }
            // laat ook even zien welke varkentjes hij ruikt
            for (auto i = begin_perceived(); i != end_perceived(); ++i) {
                auto const &a = *i;
                std::cout << "Smelled a " + a.getName() + " at " << a.getLocation().x() << ", "
                          << a.getLocation().y() << "\n";
            }
        }

    } // namespace pigisland
} // namespace kmint
