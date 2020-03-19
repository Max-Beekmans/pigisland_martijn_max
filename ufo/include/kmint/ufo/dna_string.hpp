#ifndef UFO_MARTIJN_MAX_DNA_STRING_HPP
#define UFO_MARTIJN_MAX_DNA_STRING_HPP

#include <memory>
#include <vector>

namespace kmint::ufo {
    class dna_string {
    public:
        typedef std::vector<double> chromosome;
        dna_string(double attractionToRedTank, double attractionToGreenTank, double attractionToUFO, double attractionToDoors, double cohesion, double separation, double alignment );
        dna_string();

        static std::unique_ptr<dna_string> createRandom();
        chromosome getChromosome() const;
        void setChromosome(chromosome const &c);
        dna_string operator*(dna_string const &other) const;

        double getAttractionToRedTank() const;
        void setAttractionToRedTank(double attractionToRedTank);
        double getAttractionToGreenTank() const;
        void setAttractionToGreenTank(double attractionToGreenTank);
        double getAttractionToUFO() const;
        void setAttractionToUFO(double attractionToUFO);
        double getAttractionToDoors() const;
        void setAttractionToDoors(double attractionToDoors);
        double getCohesion() const;
        void setCohesion(double cohesion);
        double getSeparation() const;
        void setSeparation(double separation);
        double getAlignment() const;
        void setAlignment(double alignment);
    private:
        double attractionToRedTank_;
        double attractionToGreenTank_;
        double attractionToUFO_;
        double attractionToDoors_;
        double cohesion_;
        double separation_;
        double alignment_;
    };
}

#endif //UFO_MARTIJN_MAX_DNA_STRING_HPP
