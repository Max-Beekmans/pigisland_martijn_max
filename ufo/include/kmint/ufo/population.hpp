#ifndef UFO_MARTIJN_MAX_POPULATION_HPP
#define UFO_MARTIJN_MAX_POPULATION_HPP

#include <map>
#include "kmint/play.hpp"
#include "kmint/ufo/human.hpp"

namespace kmint::ufo {

    class population {
        public:
            explicit population(play::stage &s);
            std::vector<human*> populate();
        private:
            std::map<human*, double> generatePopulateChances();
            human* chooseHuman(double random, std::map<human*, double> &chances);
            play::stage& stage;
    };

}

#endif //UFO_MARTIJN_MAX_POPULATION_HPP
