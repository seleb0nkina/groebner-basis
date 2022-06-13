#ifndef POLYNOMIALSET_H
#define POLYNOMIALSET_H

#include <set>
#include "polynomial.h"
#include "deglex.h"
#include "lex.h"
#include "revlex.h"

template<typename T, typename Y = DegLex<T>>
class PolynomialSet : public std::set<Polynomial<T, Y>> {

};

#endif //POLYNOMIALSET_H