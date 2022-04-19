#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include "deglex.h"
#include <algorithm>

template<typename T>
class Polynomial {
public:
    Polynomial() = default;

    Polynomial(const Polynomial &other);

    Polynomial(Polynomial &&other);

    Polynomial operator+(const Polynomial &other) const;

    Polynomial operator-(const Polynomial &other) const;

    Polynomial &operator+=(const Polynomial &other);

    Polynomial &operator-=(const Polynomial &other);

    bool operator==(const Polynomial &other) const;

    bool operator!=(const Polynomial &other) const;

    Polynomial operator*(const Polynomial &other) const;

    Polynomial &operator*=(const Polynomial &other) const;

    friend std::ostream &operator<<(std::ostream &out, const Polynomial &other) {
        for (auto it = other.poly_.begin(); it != other.poly_.end(); ++it) {
            out << *it;
            if (it != other.poly_.end()) {
                out << " + ";
            }
        }
        out << std::endl;
        return out;
    }

private:
    std::vector<DegLex<T>> poly_;
};

template<typename T>
Polynomial<T>::Polynomial(const Polynomial &other) {
    poly_ = other.poly_;
}

template<typename T>
Polynomial<T>::Polynomial(Polynomial &&other) {
    poly_ = other.poly_;
}

template<typename T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial &other) const {
    std::vector<DegLex<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), other.begin(), other.end(), std::back_inserter(monomials.begin()));
    std::vector<DegLex<T>> tmp;
    for (const auto &i: monomials) {
        if (tmp.empty()) {
            tmp.push_back(i);
        } else {
            if (tmp.back() == i) {
                tmp.back() += i;
            } else {
                tmp.push_back(i);
            }
        }
    }
    return tmp;
}

template<typename T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial &other) const {
    Polynomial minus_other = other;
    for (auto &monomial: minus_other.poly_) {
        monomial *= -1;
    }
    std::vector<DegLex<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), minus_other.beginother(), minus_other.end(),
               std::back_inserter(monomials.begin()));
    std::vector<DegLex<T>> tmp;
    for (const auto &i: monomials) {
        if (tmp.empty()) {
            tmp.push_back(i);
        } else {
            if (tmp.back() == i) {
                tmp.back() += i;
            } else {
                tmp.push_back(i);
            }
        }
    }
    return tmp;
}

template<typename T>
Polynomial<T> &Polynomial<T>::operator+=(const Polynomial &other) {
    std::vector<DegLex<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), other.begin(), other.end(), std::back_inserter(monomials.begin()));
    std::vector<DegLex<T>> tmp;
    for (const auto &i: monomials) {
        if (tmp.empty()) {
            tmp.push_back(i);
        } else {
            if (tmp.back() == i) {
                tmp.back() += i;
            } else {
                tmp.push_back(i);
            }
        }
    }
    this->poly = tmp;
    return *this;
}

template<typename T>
Polynomial<T> &Polynomial<T>::operator-=(const Polynomial &other) {
    Polynomial minus_other = other;
    for (auto &monomial: minus_other.poly_) {
        monomial *= -1;
    }
    std::vector<DegLex<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), minus_other.beginother(), minus_other.end(),
               std::back_inserter(monomials.begin()));
    std::vector<DegLex<T>> tmp;
    for (const auto &i: monomials) {
        if (tmp.empty()) {
            tmp.push_back(i);
        } else {
            if (tmp.back() == i) {
                tmp.back() += i;
            } else {
                tmp.push_back(i);
            }
        }
    }
    this->poly = tmp;
    return *this;
}

template<typename T>
bool Polynomial<T>::operator==(const Polynomial &other) const {
    if (poly_.size() != other.poly_.size()) return false;
    for (size_t i = 0; i < poly_.size(); ++i) {
        if (poly_[i] != other.poly_[i]) return false;
    }
    return true;
}

template<typename T>
bool Polynomial<T>::operator!=(const Polynomial &other) const {
    return !(*this == other);
}

template<typename T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial &other) const {
    std::vector<DegLex<T>> monomials;
    for (const auto &i: this->poly_) {
        for (const auto &j: other.poly_) {
            monomials.push_back(i * j);
        }
    }
    sort(monomials.begin(), monomials.end());
    std::vector<DegLex<T>> tmp;
    for (const auto &i: monomials) {
        if (tmp.empty()) {
            tmp.push_back(i);
        } else {
            if (tmp.back() == i) {
                tmp.back() += i;
            } else {
                tmp.push_back(i);
            }
        }
    }
    return tmp;
}

template<typename T>
Polynomial<T> &Polynomial<T>::operator*=(const Polynomial &other) const {
    std::vector<DegLex<T>> monomials;
    for (const auto &i: this->poly_) {
        for (const auto &j: other.poly_) {
            monomials.push_back(i * j);
        }
    }
    sort(monomials.begin(), monomials.end());
    std::vector<DegLex<T>> tmp;
    for (const auto &i: monomials) {
        if (tmp.empty()) {
            tmp.push_back(i);
        } else {
            if (tmp.back() == i) {
                tmp.back() += i;
            } else {
                tmp.push_back(i);
            }
        }
    }
    *this = tmp;
    return *this;

}

#endif // POLYNOMIAL_H
