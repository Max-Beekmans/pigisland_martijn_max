#ifndef UFO_MARTIJN_MAX_POPULATION_HPP
#define UFO_MARTIJN_MAX_POPULATION_HPP

#include <map>
#include "kmint/play.hpp"
#include "kmint/ufo/human.hpp"

namespace kmint::ufo {

    class population {
        public:
            explicit population(play::stage &s, std::vector<math::rectangle> &buildingRectangles);
            void populate();
        private:
            std::map<human*, double> generatePopulateChances();
            human* chooseHuman(double random, std::map<human*, double> &chances, human* notThisHuman = nullptr);
            play::stage& stage;
            std::vector<math::rectangle> buildingRectangles;
    };

}

#endif //UFO_MARTIJN_MAX_POPULATION_HPP
