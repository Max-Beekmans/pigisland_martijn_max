#include "kmint/ufo/population.hpp"
#include "kmint/ufo/random.hpp"

namespace kmint::ufo {

    population::population(play::stage &s) : stage(s) {}

    std::vector<kmint::ufo::human *> population::populate() {
        auto chances = generatePopulateChances();
        using pair = std::pair<human *, double>;
        std::vector<pair> sorted(std::begin(chances), std::end(chances));
        std::sort(std::begin(sorted), std::end(sorted), [](const pair &a, const pair &b) {
            return a.second > b.second;
        });

        std::vector<human *> newHumans;
        randomize();
        auto random = pickRandomDouble(0, 1) / 100;
        human *first;
        human *second;

        //Generate new DNA strings from human population
        auto counter = 0;
        std::vector<std::unique_ptr<dna_string>> dnaList;
        for (auto h = stage.begin(); h != stage.end(); h++) {
            auto &f = *h;
            if (auto c = dynamic_cast<human *>(&f); c) {
                first = chooseHuman(random, chances);
                random = pickRandomDouble(0, 1) / 100;
                second = chooseHuman(random, chances);
                while (first == second) {
                    random = pickRandomDouble(0, 1) / 100;
                    second = chooseHuman(random, chances);
                }
            }

            dnaList.emplace_back(
                    std::make_unique<dna_string>(first->getGeneticAttributes() * second->getGeneticAttributes()));

            counter++;
        }

        counter = 0;
        for (auto h = stage.begin(); h != stage.end(); h++) {
            auto &f = *h;
            if (auto c = dynamic_cast<human *>(&f); c) {
                c->setRandomLocation();
                c->setGeneticAttributes(dnaList[counter]);

                c->setFitness(0);
                c->setIsSafe(false);
                c->setIsDead(false);

                counter++;
            }
        }

        return newHumans;
    }

    std::map<kmint::ufo::human *, double> population::generatePopulateChances() {
        //Holds the chances a human will be chosen
        auto chances = std::map<human *, double>{};
        //Holds the calculated fitness score of the humans
        auto fitness = std::map<human *, double>{};
        auto totalFitness = 0.0;
        //Calculate total and specific fitness for each human
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
        //Calculate populate chances
        for (auto h = stage.begin(); h != stage.end(); h++) {
            auto &f = *h;
            if (auto c = dynamic_cast<human *>(&f); c) {
                double chance = fitness[c] / totalFitness;
                chances[c] = chance;
            }
        }
        return chances;
    }

    kmint::ufo::human *population::chooseHuman(double random, std::map<human *, double> &chances) {
        for (auto &human : chances) {
            if (human.second <= random) {
                return human.first;
            }
        }
        return chances.begin()->first;
    }
}