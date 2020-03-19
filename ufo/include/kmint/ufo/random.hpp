#ifndef UFO_MARTIJN_MAX_RANDOM_HPP
#define UFO_MARTIJN_MAX_RANDOM_HPP

#include <random>

namespace kmint::ufo {

    static std::default_random_engine &globalEngine() {
        static std::default_random_engine u{};
        return u;
    }

    static void randomize() {
        static std::random_device rd{};
        globalEngine().seed(rd());
    }

    static double pickRandomDouble(const double min, const double max) {
        static std::uniform_real_distribution<double> d{min, max};
        return d(globalEngine());
    }

    static int pickRandomInt(const int min, const int max) {
        static std::uniform_int_distribution<> d{min, max};
        return d(globalEngine());
    }

}

#endif //UFO_MARTIJN_MAX_RANDOM_HPP
