#include "kmint/ufo/population.hpp"
#include "kmint/ufo/play.hpp"
#include "kmint/random.hpp"

namespace kmint::ufo {

    population::population(play::stage &s, std::vector<math::rectangle> &buildingRectangles) : stage(s), buildingRectangles(buildingRectangles) {}

    void population::populate() {
        auto chances = generatePopulateChances();
        using pair = std::pair<human *, double>;
        std::vector<pair> sorted(std::begin(chances), std::end(chances));
        std::sort(std::begin(sorted), std::end(sorted), [](const pair &a, const pair &b) {
            return a.second > b.second;
        });

        auto random = random_scalar(0.0, 1.0) / 100;
        human *first;
        human *second;

        //Generate new DNA strings from human population
        std::vector<dna_string> dnaList;
        for (auto h = stage.begin(); h != stage.end(); h++) {
            auto &f = *h;
            if (auto c = dynamic_cast<human *>(&f); c) {
                first = chooseHuman(random, chances);
                random = random_scalar(0.0, 1.0) / 100;
                second = chooseHuman(random, chances, first);

                //! add a new dna string to the list, calculated from the 2 parents with * operator
                dnaList.emplace_back(
                        dna_string(first->getGeneticAttributes() * second->getGeneticAttributes()));

            }
        }

        auto counter = 0;
        for (auto h = stage.begin(); h != stage.end(); h++) {
            auto &f = *h;
            if (auto c = dynamic_cast<human *>(&f); c) {
                math::vector2d newLocation{0,0};
                while(newLocation == math::vector2d{0,0}) {
                    math::vector2d randomLocation{ random_scalar(24, 1000), random_scalar(24, 744) };
                    bool contains = false;
                    for(auto it = buildingRectangles.begin(); it != buildingRectangles.end(); it++) {
                        if(math::contains(*it, randomLocation)) {
                            contains = true;
                        }
                    }
                    if(!contains) newLocation = randomLocation;
                }
                c->setRandomLocation(newLocation);
                c->setGeneticAttributes(dnaList[counter]);

                c->setFitness(200);
                c->setIsSafe(false);
                c->setIsDead(false);

                counter++;
            }
        }
    }

    std::map<kmint::ufo::human *, double> population::generatePopulateChances() {
        //! Holds the chances a human will be chosen
        auto chances = std::map<human *, double>{};
        //! Holds the calculated fitness score of the humans
        auto fitness = std::map<human *, double>{};
        auto totalFitness = 0.0;
        //! Calculate fitness for each human and add to totalFitness
        for (auto i = stage.begin(); i != stage.end(); i++) {
            auto &f = *i;
            if (auto h = dynamic_cast<human *>(&f); h) {
                auto currentFitness = 0.0;
                currentFitness += h->getFitness();
                if (h->isSafe()) {
                    currentFitness *= 10;
                } else if (h->isDead()) {
                    currentFitness *= 0.7;
                }
                fitness[h] = currentFitness;
                totalFitness += currentFitness;
            }
        }
        //! Calculate populate chances
        for (auto h = stage.begin(); h != stage.end(); h++) {
            auto &f = *h;
            if (auto c = dynamic_cast<human *>(&f); c) {
                double chance = fitness[c] / totalFitness;
                chances[c] = chance;
            }
        }
        return chances;
    }

    kmint::ufo::human *population::chooseHuman(double random, std::map<human *, double> &chances, human* notThisHuman) {
        for (auto &human : chances) {
            if (human.second <= random) {
                if(notThisHuman != nullptr && human.first != notThisHuman) {
                    return human.first;
                }
            }
        }

        auto it = chances.begin();

        while(it->first == notThisHuman ) {
            it++;
        }

        return it->first;
    }
}