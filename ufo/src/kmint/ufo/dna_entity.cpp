#include <kmint/ufo/dna_string.hpp>
#include "kmint/ufo/dna_entity.hpp"

namespace kmint::ufo {

    dna_entity::dna_entity() : geneticAttributes() {}

    void dna_entity::setGeneticAttributes(dna_string &a) {
        geneticAttributes = std::move(a);
    }

    dna_string dna_entity::getGeneticAttributes() const {
        return geneticAttributes;
    }
}