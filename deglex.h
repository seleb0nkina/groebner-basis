#ifndef DEGLEX_H
#define DEGLEX_H

#include "monomial.h"

#include <vector>

template<typename T>
class DegLex : public Monomial<T> {
public:
    using IndexType = typename Monomial<T>::IndexType;
    using DegreeType = typename Monomial<T>::DegreeType;

    DegLex(const Monomial<T> &other);

    bool operator<(const DegLex &other);

    bool operator>(const DegLex &other);

    bool operator==(const DegLex &other);

    friend std::ostream &operator<<(std::ostream &out, const DegLex &m) {
        out << m.coefficient_;
        for (const auto &p: m.powers_) {
            out << "x_" << p.first << "^" << p.second;
        }
        return out;
    }

private:
    size_t powerSum() const;

    std::vector<std::pair<IndexType, DegreeType>> getPowers() const;
};

template<typename T>
DegLex<T>::DegLex(const Monomial<T> &other) {
    this->powers_ = other.getPowers();
    this->coefficient_ = other.getCoefficient();
}

template<typename T>
size_t DegLex<T>::powerSum() const {
    size_t res = 0;
    for (const auto &j: this->powers_) {
        res += j.second;
    }
    return res;
}

template<typename T>
std::vector<std::pair<typename DegLex<T>::IndexType, typename DegLex<T>::DegreeType>> DegLex<T>::getPowers() const {
    std::vector<std::pair<IndexType, DegreeType>> res;
    for (const auto &j: this->powers_) {
        res.push_back(j);
    }
    return res;
}

template<typename T>
bool DegLex<T>::operator<(const DegLex &other) {
    size_t our = powerSum();
    size_t oth = other.powerSum();
    if (our == oth) {
        auto myPowers = this->getPowers();
        auto othPowers = other.getPowers();
        return myPowers < othPowers;
    }
    return our < oth;
}

template<typename T>
bool DegLex<T>::operator>(const DegLex &other) {
    size_t our = this->powerSum();
    size_t oth = other.powerSum();
    if (our == oth) {
        auto myPowers = this->getPowers();
        auto othPowers = other.getPowers();
        return myPowers > othPowers;
    }
    return our > oth;
}

template<typename T>
bool DegLex<T>::operator==(const DegLex &other) {
    size_t our = this->powerSum();
    size_t oth = other.powerSum();
    auto myPowers = this->getPowers();
    auto othPowers = other.getPowers();
    return our == oth && myPowers == othPowers;
}



#endif // DEGLEX_H
