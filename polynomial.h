#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <algorithm>
#include <vector>

#include "deglex.h"

template<typename T, typename Y>
class Polynomial {
public:
    Polynomial() = default;

    Polynomial(const Polynomial &other);

    Polynomial(Polynomial &&other);

    Polynomial(const std::vector<Monomial<T>> &oth);

    Polynomial operator+(const Polynomial &other) const;

    Polynomial operator-(const Polynomial &other) const;

    Polynomial &operator+=(const Polynomial &other);

    Polynomial &operator-=(const Polynomial &other);

    bool operator==(const Polynomial &other) const;

    bool operator!=(const Polynomial &other) const;

    Polynomial operator*(const Polynomial &other) const;

    Polynomial &operator*=(const Polynomial &other) const;

    bool operator<(const Polynomial &other) const;

    friend std::ostream &operator<<(std::ostream &out, const Polynomial &other) {
        for (auto it = other.poly_.begin(); it != other.poly_.end(); ++it) {
            out << *it;
            auto temp = it;
            ++temp;
            if (temp != other.poly_.end()) {
                out << " + ";
            }
        }
        return out;
    }

    Monomial<T> LargestTerm() const {
        if (poly_.empty()) {
            throw;
        }
        return poly_.back();
    }

    void addMonomial(const Monomial<T> &other) {
        for (size_t i = 0 ; i < poly_.size(); ++i) {
            if(poly_[i].getPowers() == other.getPowers()) {
                poly_[i].addCoefficient(other.getCoefficient());
                return;
            }
        }
        for (size_t i = 0; i < poly_.size(); ++i) {
            if(Y()(other, poly_[i])) {
                poly_.insert(poly_.begin() + i, other);
                return;
            }
        }
        poly_.push_back(other);
    }
private:
    std::vector<Monomial<T>> poly_;
};

template<typename T, typename Y>
Polynomial<T, Y>::Polynomial(const Polynomial &other) {
    poly_ = other.poly_;
}

template<typename T, typename Y>
Polynomial<T, Y>::Polynomial(Polynomial &&other) {
    poly_ = other.poly_;
}

template<typename T, typename Y>
Polynomial<T, Y>::Polynomial(const std::vector<Monomial<T>> &oth) {
    poly_ = {};
    for (auto i : oth) {
        addMonomial(i);
    }
}

template<typename T, typename Y>
Polynomial<T, Y> Polynomial<T, Y>::operator+(const Polynomial &other) const {
    Polynomial<T, Y> tmp = *this;
    tmp += other;
    return tmp;
}

template<typename T, typename Y>
Polynomial<T, Y> Polynomial<T, Y>::operator-(const Polynomial &other) const {
    Polynomial<T, Y> tmp = *this;
    tmp -= other;
    return tmp;
}

template<typename T, typename Y>
Polynomial<T, Y> &Polynomial<T, Y>::operator+=(const Polynomial &other) {
    std::vector<Monomial<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), other.begin(), other.end(), std::back_inserter(monomials.begin()),
               Y());
    std::vector<Monomial<T>> tmp;
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
    poly_ = tmp;
    return *this;
}

template<typename T, typename Y>
Polynomial<T, Y> &Polynomial<T, Y>::operator-=(const Polynomial &other) {
    Polynomial minus_other = other;
    for (auto &monomial: minus_other.poly_) {
        monomial *= -1;
    }
    std::vector<Monomial<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), minus_other.beginother(), minus_other.end(),
               std::back_inserter(monomials.begin()), Y());
    std::vector<Monomial<T>> tmp;
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
    poly_ = tmp;
    return *this;
}

template<typename T, typename Y>
bool Polynomial<T, Y>::operator==(const Polynomial &other) const {
    return poly_ == other.poly_;
}

template<typename T, typename Y>
bool Polynomial<T, Y>::operator!=(const Polynomial &other) const {
    return poly_ != other.poly_;
}

template<typename T, typename Y>
Polynomial<T, Y> Polynomial<T, Y>::operator*(const Polynomial &other) const {
    Polynomial<T, Y> tmp = *this;
    tmp *= other;
    return tmp;
}

template<typename T, typename Y>
Polynomial<T, Y> &Polynomial<T, Y>::operator*=(const Polynomial &other) const {
    std::vector<Monomial<T>> monomials;
    for (const auto &i: this->poly_) {
        for (const auto &j: other.poly_) {
            monomials.push_back(i * j);
        }
    }
    sort(monomials.begin(), monomials.end(), Y());
    std::vector<Monomial<T>> tmp;
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

template<typename T, typename Y>
bool Polynomial<T, Y>::operator<(const Polynomial &other) const {
    size_t minlen = std::min(other.poly_.size(), poly_.size());
    for (size_t i = 0; i < minlen; ++i)
    {
        if (other.poly_[other.poly_.size() - i - 1] == poly_[poly_.size() - i - 1]) {
            continue;
        }
        if(Y()(other.poly_[other.poly_.size() - i - 1], poly_[poly_.size() - i - 1])) {
            return 0;
        }
        return 1;
    }
    return poly_.size() < other.poly_.size();
}

#endif // POLYNOMIAL_H