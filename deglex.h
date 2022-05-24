#ifndef DEGLEX_H
#define DEGLEX_H

#include "monomial.h"

#include <vector>

template<typename T>
class DegLex {
public:
    bool operator()(const Monomial<T>& a, const Monomial<T>& b) const {
        size_t our = a.powerSum();
        size_t oth = b.powerSum();
        if (our == oth) {
            auto myPowers = a.getPowers();
            auto othPowers = b.getPowers();
            return (myPowers < othPowers || a.getCoefficient() < b.getCoefficient());
        }
        return our < oth;
    }
};

#endif // DEGLEX_H
