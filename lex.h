#ifndef LEX_H
#define LEX_H

#include "monomial.h"

template<typename T>
class Lex {
public:
    bool operator()(const Monomial<T> &a, const Monomial<T> &b) const {
        auto our = a.getPowers();
        auto oth = b.getPowers();
        return (our < oth || our == oth && a.getCoefficient() < b.getCoefficient());
    }
};

#endif // LEX_H