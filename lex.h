#ifndef LEX_H
#define LEX_H

#include "monomial.h"

template<typename T>
class Lex {
public:
    bool operator()(const T &a, const T &b) const {
        auto our = a.getPowers();
        auto oth = b.getPowers();
        return our < oth;
    }
};

#endif // LEX_H