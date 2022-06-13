#ifndef REVLEX_H
#define REVLEX_H

#include "monomial.h"
#include <algorithm>

template<typename T>
class RevLex {
public:
    RevLex() {}

    bool operator()(const Monomial<T> &a, const Monomial<T> &b) const {
        auto our = a.getPowers();
        auto oth = b.getPowers();
        std::reverse(our.begin(), our.end());
        std::reverse(oth.begin(), oth.end());
        return (our < oth || a.getCoefficient() < b.getCoefficient());
    }
};

#endif // REVLEX_H