#ifndef MONOMIAL_H
#define MONOMIAL_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

template<typename T>
class Monomial {
public:
    using IndexType = unsigned int;
    using DegreeType = int;
    using PowersType = std::map<IndexType, DegreeType>;

    Monomial() = default;

    Monomial(const T &coefficient, std::initializer_list<std::pair<IndexType, DegreeType>> l) {
        coefficient_ = coefficient;
        for (const auto &j: l) {
            powers_[j.first] = j.second;
        }
    }

    Monomial &operator*=(const Monomial &other) {
        coefficient_ *= other.coefficient_;
        for (const auto &j: other.powers_) {
            powers_[j.first] += j.second;
        }
        return *this;
    }

    bool isDivisibleBy(const Monomial &other) const {
        for (const auto &j: other.powers_) {
            auto power = powers_.find(j.first);
            if (power == powers_.end() || power->second < j.second) {
                return false;
            }
        }
        return true;
    }

    Monomial &operator/=(const Monomial &other) {
        if (!isDivisibleBy(other)) {
            //
        }
        coefficient_ /= other.coefficient_;
        for (const auto &j: other.powers_) {
            auto power = powers_.find(j.first);

            power->second -= j.second;
            if (power->second == 0) {
                powers_.erase(power);
            }
        }
        assert(isStateCorrect());

        return *this;
    }

    Monomial operator*(const Monomial &other) const {
        Monomial temp = *this;
        temp *= other;
        return temp;
    }

    Monomial operator+(const Monomial &other) const {
        if (this->powers_ != other.powers_) {
            throw std::runtime_error("skladyvaem nepravilnye monomyy");
        }
        Monomial *temp = *this;
        temp->coefficient_ += other.coefficient_;
        return temp;
    }

    Monomial operator-(const Monomial &other) const {
        if (this->powers_ != other.powers_) {
            throw std::runtime_error("vichytaem nepravilnye monomyy");
        }
        Monomial *temp = *this;
        temp->coefficient_ -= other.coefficient_;
        return temp;
    }

    Monomial& operator+=(const Monomial &other) {
        if (this->powers_ != other.powers_) {
            throw std::runtime_error("skladyvaem nepravilnye monomyy");
        }
        this->coefficient_ += other.coefficient_;
        return *this;
    }

    Monomial& operator-=(const Monomial &other) {
        if(this->powers_!=other.powers_ ) {
            throw std::runtime_error("vichytaem nepravilno");
        }
        this->coefficient_ -= other.coefficient_;
        return *this;
    }

    Monomial operator/(const Monomial &other) const {
        Monomial temp = *this;
        temp /= other;
        return temp;
    }

    Monomial &operator=(const Monomial &other) {
        coefficient_ = other.coefficient_;
        powers_ = other.powers_;
        return *this;
    }

    T getCoefficient() const {
        return coefficient_;
    }

    PowersType getPowers() const {
        return powers_;
    }

    bool isStateCorrect() const {
        for (const auto &p: powers_) {
            if (p.second <= 0) {
                return false;
            }
        }
        return true;
    }

    friend std::ostream &operator<<(std::ostream &out, const Monomial &m) {
        out << m.coefficient_;
        for (const auto &p: m.powers_) {
            out << "x_" << p.first << "^" << p.second;
        }
        return out;
    }

protected:
    T coefficient_ = T(0);
    PowersType powers_;
};

#endif // MONOMIAL_H
