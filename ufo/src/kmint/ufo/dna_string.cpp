#include <memory>
#include "kmint/ufo/dna_string.hpp"
#include "kmint/ufo/random.hpp"

namespace kmint::ufo {
    dna_string::dna_string(double attractionToRedTank, double attractionToGreenTank, double attractionToUFO,
            double attractionToDoors, double cohesion, double separation, double alignment) :
            attractionToRedTank_(attractionToRedTank), attractionToGreenTank_(attractionToGreenTank),
            attractionToUFO_(attractionToUFO), attractionToDoors_(attractionToDoors), cohesion_(cohesion),
            separation_(separation), alignment_(alignment) {};

    dna_string::dna_string() : attractionToRedTank_(0), attractionToGreenTank_(0), attractionToUFO_(0),
        attractionToDoors_(0), cohesion_(0), separation_(0), alignment_(0) {};

    double dna_string::getAttractionToRedTank() const {
        return attractionToRedTank_;
    }

    void dna_string::setAttractionToRedTank(double attractionToRedTank) {
        attractionToRedTank_ = attractionToRedTank;
    }

    double dna_string::getAttractionToGreenTank() const {
        return attractionToGreenTank_;
    }

    void dna_string::setAttractionToGreenTank(double attractionToGreenTank) {
        attractionToGreenTank_ = attractionToGreenTank;
    }

    double dna_string::getAttractionToUFO() const {
        return attractionToUFO_;
    }

    void dna_string::setAttractionToUFO(double attractionToUFO) {
        attractionToUFO_ = attractionToUFO;
    }

    double dna_string::getAttractionToDoors() const {
        return attractionToDoors_;
    }

    void dna_string::setAttractionToDoors(double attractionToDoors) {
        attractionToDoors_ = attractionToDoors;
    }

    double dna_string::getCohesion() const {
        return cohesion_;
    }

    void dna_string::setCohesion(double cohesion) {
        cohesion_ = cohesion;
    }

    double dna_string::getSeparation() const {
        return separation_;
    }

    void dna_string::setSeparation(double separation) {
        separation_ = separation;
    }

    double dna_string::getAlignment() const {
        return alignment_;
    }

    void dna_string::setAlignment(double alignment) {
        alignment_ = alignment;
    }

    std::unique_ptr<dna_string> dna_string::createRandom() {
        return std::make_unique<dna_string>(
                    pickRandomDouble(-1, 1),
                    pickRandomDouble(-1, 1),
                    pickRandomDouble(-1, 1),
                    pickRandomDouble(-1, 1),
                    pickRandomDouble(0, 1),
                    pickRandomDouble(0, 1),
                    pickRandomDouble(0, 1)
                );
    }

    dna_string::chromosome dna_string::getChromosome() const {
        auto c = chromosome{};
        c.emplace_back(getAttractionToRedTank());
        c.emplace_back(getAttractionToGreenTank());
        c.emplace_back(getAttractionToUFO());
        c.emplace_back(getAttractionToDoors());
        c.emplace_back(getCohesion());
        c.emplace_back(getSeparation());
        c.emplace_back(getAlignment());

        return c;
    }

    void dna_string::setChromosome(const kmint::ufo::dna_string::chromosome &c) {
        setAttractionToRedTank(c[0]);
        setAttractionToGreenTank(c[1]);
        setAttractionToUFO(c[2]);
        setAttractionToDoors(c[3]);
        setCohesion(c[4]);
        setSeparation(c[5]);
        setAlignment(c[6]);
    }

    dna_string dna_string::operator*(dna_string const &other) const {
        randomize();
        auto newDNAString = dna_string{};
        auto newChromosome = chromosome{};
        auto chromosome1 = getChromosome();
        auto chromosome2 = other.getChromosome();
        newChromosome.resize(chromosome1.size());

        auto split = pickRandomInt(1, static_cast<int>(chromosome1.size() - 1));
        for(auto i = 0; i < split; i++) {
            newChromosome[i] = chromosome1[i];
        }
        for(auto i = split; i < chromosome1.size(); i++) {
            newChromosome[i] = chromosome2[i];
        }

        randomize();
        auto r = pickRandomInt(0, 99);
        auto toMutate = (pickRandomInt(0, 99) < 5);
        if(toMutate) {
            auto mutateIndex = pickRandomInt(0, static_cast<int>(chromosome1.size()) - 1);
            auto mutateAmount = pickRandomDouble(0, 1);
            newChromosome[mutateIndex] = mutateAmount;
        }

        newDNAString.setChromosome(newChromosome);

        return newDNAString;
    }


}