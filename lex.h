#ifndef LEX_H
#define LEX_H

#include "monomial.h"

template<typename T>
class Lex {
public:
    Lex() {}

    bool operator()(const Monomial<T> &a, const Monomial<T> &b) const {
        auto our = a.getPowers();
        auto oth = b.getPowers();
        if (our == oth) {
            return a.getCoefficient() < b.getCoefficient();
        }
        for (int z = 0;; ++z) {
            if (our[z] != oth[z]) {
                if (our[z] < oth[z]) {
                    return 1;
                }
                return 0;
            }
        }
    }
};

#endif // LEX_H