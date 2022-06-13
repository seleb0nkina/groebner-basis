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
            if (powers_[j.first] == 0) {
                powers_.erase(j.first);
            }
        }
        assert(isStateCorrect());
        return *this;
    }

    bool operator==(const Monomial &other) const {
        return powers_ == other.powers_;
    }

    bool fullEqual(const Monomial &other) const {
        return other == (*this) && other.coefficient_ == this->coefficient_;
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
            throw std::runtime_error("Incorrect division");
        }
        coefficient_ /= other.coefficient_;
        for (const auto &j: other.powers_) {
            powers_[j.first] -= j.second;
            if (powers_[j.first] == 0) {
                powers_.erase(j.first);
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
            throw std::runtime_error("Incorrect addition");
        }
        Monomial temp = *this;
        temp.coefficient_ += other.coefficient_;
        return temp;
    }

    Monomial operator-(const Monomial &other) const {
        if (this->powers_ != other.powers_) {
            throw std::runtime_error("Incorrect substraction");
        }
        Monomial temp = *this;
        temp.coefficient_ -= other.coefficient_;
        return temp;
    }

    Monomial &operator+=(const Monomial &other) {
        if (this->powers_ != other.powers_) {
            throw std::runtime_error("Incorrect addition");
        }
        this->coefficient_ += other.coefficient_;
        return *this;
    }

    Monomial &operator-=(const Monomial &other) {
        if (this->powers_ != other.powers_) {
            throw std::runtime_error("Incorrect substraction");
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

    void addCoefficient(const T &other) {
        coefficient_ += other;
    }

    PowersType getPowers() const {
        return powers_;
    }

    DegreeType powerSum() const {
        DegreeType sum = 0;
        for (auto &j: powers_) {
            sum += j.second;
        }
        return sum;
    }

    bool isStateCorrect() const {
        for (const auto &p: powers_) {
            if (p.second <= 0) {
                return false;
            }
        }
        return true;
    }

    Monomial LCM(const Monomial &other) {
        Monomial res;
        res.coefficient_ = 1;
        for (auto i: powers_) {
            res.powers_[i.first] = std::max(res.powers_[i.first], i.second);
        }
        for (auto i: other.powers_) {
            res.powers_[i.first] = std::max(res.powers_[i.first], i.second);
        }
        assert(res.isStateCorrect());
        return res;
    }

    friend std::ostream &operator<<(std::ostream &out, const Monomial &m) {
        out << m.coefficient_;
        for (const auto &p: m.powers_) {
            out << "x_" << p.first << "^" << p.second;
        }
        return out;
    }

protected:
    T coefficient_ = T(1);
    PowersType powers_;
};

#endif // MONOMIAL_H