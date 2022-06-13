#ifndef ALGOS_H
#define ALGOS_H
using namespace std;

#include "polynomialset.h"

template<typename T, typename Y>
bool criteria(size_t i, size_t j,
              const std::set<std::pair<size_t, size_t>> &B, const std::vector<Polynomial<T, Y>> &toIterate) {
    auto tlc = toIterate[i].LeadingTerm().LCM(toIterate[j].LeadingTerm());
    for (size_t l = 0; l < toIterate.size(); ++l) {
        if (B.count(std::make_pair(i, l)) || B.count(std::make_pair(j, l)) || B.count(std::make_pair(l, i)) ||
            B.count(std::make_pair(l, j))) {
            continue;
        }
        if (tlc.isDivisibleBy(toIterate[l].LeadingTerm())) {
            return true;
        }
    }
    return false;
}

template<typename T, typename Y>
Polynomial<T, Y> polyS(const Polynomial<T, Y> &f, const Polynomial<T, Y> &g) {
    auto fst = f.LeadingTerm(), sect = g.LeadingTerm();
    auto lc = fst.LCM(sect);
    lc.addCoefficient(1 - lc.getCoefficient());
    assert(lc.getCoefficient() == T(1));
    return f * Polynomial<T, Y>(std::vector<Monomial<T>>{lc / fst}) -
           g * Polynomial<T, Y>(std::vector<Monomial<T>>{lc / sect});
}

template<typename T, typename Y>
Polynomial<T, Y> normalPoly(const Polynomial<T, Y> &f) {
    auto copy_f = f;
    auto to_divide = copy_f.back().coefficient;
    for (auto &i: copy_f) {
        i.coefficient /= to_divide;
    }
    return copy_f;

}

template<typename T, typename Y>
Polynomial<T, Y> getReminder(const PolynomialSet<T, Y> &F, const Polynomial<T, Y> &f) {
    Polynomial<T, Y> r;
    Polynomial<T, Y> p = f;
    while (p.getSize() != 0) {
        auto i_iter = F.begin();
        bool division = false;
        while (i_iter != F.end() && !division) {
            if ((p.LeadingTerm()).isDivisibleBy((*i_iter).LeadingTerm())) {
                p -= Polynomial<T, Y>(std::vector<Monomial<T>>{p.LeadingTerm() / (*i_iter).LeadingTerm()}) * (*i_iter);
                division = 1;
            } else {
                ++i_iter;
            }
        }
        if (!division) {
            r += Polynomial<T, Y>(std::vector<Monomial<T>>{p.LeadingTerm()});
            p -= Polynomial<T, Y>(std::vector<Monomial<T>>{p.LeadingTerm()});
        }
    }
    return r;
}

template<typename T, typename Y>
PolynomialSet<T, Y> buchberger(PolynomialSet<T, Y> &F) {
    std::set<std::pair<size_t, size_t>> B;
    for (size_t i = 0; i < F.size(); ++i) {
        for (size_t j = i + 1; j < F.size(); ++j) {
            B.insert(std::make_pair(i, j));
        }
    }
    PolynomialSet<T, Y> G;
    G = F;
    size_t t = F.size();
    std::vector<Polynomial<T, Y>> toIterate(F.begin(), F.end());
    while (B.size()) {
        auto now = *B.begin();
        if (!(toIterate[now.first].LeadingTerm().LCM(
                toIterate[now.second].LeadingTerm()) ==
              toIterate[now.first].LeadingTerm() *
              toIterate[now.second].LeadingTerm()) &&
            !criteria(now.first, now.second, B, toIterate)) {
            auto S = getReminder(G, polyS(toIterate[now.first], toIterate[now.second]));
            if (S.getSize() > 0) {
                ++t;
                F.insert(S);
                toIterate.push_back(S);
                for (size_t x = 0; x < t - 1; ++x) {
                    B.insert(std::make_pair(x, t - 1));
                }
                G.insert(S);
            }
        }
        B.erase(now);
    }
    return G;
}

template<typename T, typename Y>
bool liesInIdeal(const Polynomial<T, Y> &f, const PolynomialSet<T, Y> &I) {
    auto tI = I;
    tI = buchberger(tI);
    auto now = getReminder(tI, f);
    return now.getSize() == 0;
}

template<typename T, typename Y>
bool belongToIdeal(const PolynomialSet<T, Y> &I, const Polynomial<T, Y> &p) {
    auto leading = p.LeadingTerm();
    for (auto iter = I.begin(); iter != I.end(); ++iter) {
        if (leading.isDivisibleBy(iter->LeadingTerm())) {
            return true;
        }
    }
    return false;
}


template<typename T, typename Y>
PolynomialSet<T, Y> reduceBasis(const PolynomialSet<T, Y> &F) {
    auto copyF = F;
    PolynomialSet<T, Y> normalizedF;
    for (const auto &i: F) {
        normalizedF.insert(normalPoly(i));
    }
    auto normCopy = normalizedF;
    for (const auto &i: normCopy) {
        normalizedF.erase(i);
        if (belongToIdeal(normalizedF, i)) {
            continue;
        }
        normalizedF.insert(i);
    }
    normCopy = normalizedF;
    for (const auto &i: normCopy) {
        normalizedF.erase(i);
        normalizedF.insert(getReminder(normalizedF, i));
    }
    return normalizedF;
}


#endif //ALGOS_H