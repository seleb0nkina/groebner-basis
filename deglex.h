#ifndef DEGLEX_H
#define DEGLEX_H

#include "monomial.h"

#include <vector>

template<typename T>
class DegLex {
public:
    DegLex() {}

    bool operator()(const Monomial<T> &a, const Monomial<T> &b) const {
        size_t our = a.powerSum();
        size_t oth = b.powerSum();
        if (our == oth) {
            auto myPowers = a.getPowers();
            auto othPowers = b.getPowers();
            if (myPowers == othPowers) {
                return a.getCoefficient() < b.getCoefficient();
            }
            for (int z = 0;; ++z) {
                if (myPowers[z] != othPowers[z]) {
                    if (myPowers[z] < othPowers[z]) {
                        return 1;
                    }
                    return 0;
                }
            }
        }
        return our < oth;
    }
};

#endif // DEGLEX_H