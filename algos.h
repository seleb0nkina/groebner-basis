#ifndef ALGOS_H
#define ALGOS_H

#include "polynomialset.h"

template<typename T, typename Y>
bool criteria(size_t i, size_t j,
              const std::set<std::pair<size_t,size_t>> B, const std::vector<Polynomial<T, Y>> &toIterate) {
    auto tlc = Monomial<T>::LCM(Polynomial<T, Y>::LargestTerm(toIterate[i]),
                                Polynomial<T, Y>::LargestTerm(toIterate[j]));
    for (size_t l = 0; l < toIterate.size(); ++l) {
        if (B.count(std::make_pair(i, l)) || B.count(std::make_pair(j, l))) {
            continue;
        }
        if (isDivisibleBy(tlc, Polynomial<T, Y>::LargestTerm(toIterate[l]))) {
            return true;
        }
    }
    return false;
}

template<typename T, typename Y>
Polynomial<T, Y> polyS(const Polynomial<T, Y>& f, const Polynomial<T, Y>& g) {
    auto fst = Polynomial<T, Y>::LargestTerm(f), sect = Polynomial<T, Y>::LargestTerm(g);
    auto lc = Monomial<T>::LCM(fst, sect);
    lc.setCoefficient(1);
    return f * Polynomial<T, Y>(std::vector<Monomial<T>>(lc / fst)) -
           g * Polynomial<T, Y>(std::vector<Monomial<T>>(lc / sect));
}

template<typename T, typename Y>
Polynomial<T, Y> normalPoly(const Polynomial<T, Y> &f) {
    auto copy_f = f;
    auto to_divide = copy_f.back().coefficient;
    for (auto &i : copy_f) {
        i.coefficient /= to_divide;
    }
    return copy_f;

}

template<typename T, typename Y>
Polynomial<T, Y> getReminder(const PolynomialSet<T, Y>& F, const Polynomial<T, Y>& f) {
    Polynomial<T, Y> r;
    Polynomial<T, Y> p = f;
    while (p.getSize() != 0) {
        size_t i = 0;
        auto i_iter = F.begin();
        bool division = false;
        while (i_iter != F.end() && !division) {
            if (Monomial<T>::isDisibleBy(Polynomial<T, Y>::LargestTerm(*i_iter), Polynomial<T, Y>::LargestTerm(p))) {
                p -= Polynomial<T, Y>::LargestTerm(p) / Polynomial<T, Y>::LargestTerm(*i_iter) * (*i_iter);
                division = 1;
            }
            else {
                ++i_iter;
            }
        }
        if (!division) {
            r += Polynomial<T, Y>(std::vector<Monomial<T>>(Polynomial<T, Y>::LargestTerm(p)));
            p -= Polynomial<T, Y>(std::vector<Monomial<T>>(Polynomial<T, Y>::LargestTerm(p)));
        }
    }
    return r;
}

template<typename T, typename Y>
PolynomialSet<T, Y> buchberger(PolynomialSet<T, Y> &F) {
    std::set<std::pair<size_t, size_t>> B;
    for(size_t i = 0; i < F.size(); ++i) {
        for(size_t j = i + 1; j < F.size(); ++j) {
            B.insert(std::make_pair(i, j));
        }
    }
    auto G = F;
    size_t t = F.size();
    while(B.size()) {
        auto now = *B.begin();
        std::vector<Polynomial<T, Y>> toIterate(F.begin(), F.end());
        if(!(Monomial<T>::LCM(Polynomial<T,Y>::LargestTerm(toIterate[now.first]),
                              Polynomial<T,Y>::LargestTerm(toIterate[now.second]))) ==
           Polynomial<T,Y>::LargestTerm(toIterate[now.first]) *
           Polynomial<T,Y>::LargestTerm(toIterate[now.second]) &&
           !criteria(now.first, now.second, B, toIterate)) {
            auto S = getReminder(G, polyS(toIterate[now.first], toIterate[now.second]));
            if (S.getSize() > 0) {
                ++t;
                F.insert(S);
                for (size_t x = 0; x < t - 1; ++x)
                {
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
bool liesInIdeal(const Polynomial<T, Y>& f, const PolynomialSet<T, Y>& I) {
    auto tI = I;
    tI = buchberger(tI);
    auto now = getReminder(tI, f);
    return now.getSize() == 0;
}

template<typename T, typename Y>
bool belongToIdeal(const PolynomialSet<T, Y> &I, const Polynomial<T, Y> & p) {
    auto leading = p.LargestTerm();
    for(auto iter = I.begin(); iter != I.end(); ++iter) {
        if(leading.isDivisibleBy(iter->LargestTerm())) {
            return true;
        }
    }
    return false;
}


template<typename T, typename Y>
PolynomialSet<T, Y> reduceBasis(const PolynomialSet<T, Y> &F) {
    auto copyF = F;
    PolynomialSet<T, Y> normalizedF;
    for (const auto &i : F) {
        normalizedF.insert(normalPoly(i));
    }
    auto normCopy = normalizedF;
    for (const auto &i : normCopy) {
        normalizedF.erase(i);
        if(belongToIdeal(normalizedF, i)) {
            continue;
        }
        normalizedF.insert(i);
    }
    normCopy = normalizedF;
    for(const auto &i : normCopy) {
        normalizedF.erase(i);
        normalizedF.insert(getReminder(normalizedF, i));
    }
    return normalizedF;
}


#endif //ALGOS_H
