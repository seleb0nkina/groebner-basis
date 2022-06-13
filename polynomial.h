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

    Polynomial& operator=(const Polynomial &oth);

    Polynomial operator+(const Polynomial &other) const;

    Polynomial operator-(const Polynomial &other) const;

    Polynomial &operator+=(const Polynomial &other);

    Polynomial &operator-=(const Polynomial &other);

    bool operator==(const Polynomial &other) const;

    bool operator!=(const Polynomial &other) const;

    Polynomial operator*(const Polynomial &other) const;

    Polynomial &operator*=(const Monomial<T> &oth);

    Polynomial operator*(const Monomial<T> &oth);

    Polynomial &operator*=(const Polynomial &other);

    bool operator<(const Polynomial &other) const;

    size_t getSize() const {
        return poly_.size();
    }

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

    Monomial<T> LeadingTerm() const {
        if (poly_.empty()) {
            throw;
        }
        return poly_.back();
    }

    void addMonomial(const Monomial<T> &other) {
        for (size_t i = 0; i < poly_.size(); ++i) {
            if (poly_[i].getPowers() == other.getPowers()) {
                poly_[i].addCoefficient(other.getCoefficient());
                sort(poly_.begin(), poly_.end(), Y());
                return;
            }
        }
        for (size_t i = 0; i < poly_.size(); ++i) {
            if (Y()(other, poly_[i])) {
                poly_.insert(poly_.begin() + i, other);
                sort(poly_.begin(), poly_.end(), Y());
                return;
            }
        }
        poly_.push_back(other);
        sort(poly_.begin(), poly_.end(), Y());
    }

private:
    std::vector<Monomial<T>> poly_;
};

template<typename T, typename Y>
Polynomial<T, Y>::Polynomial(const Polynomial &other) {
    poly_ = other.poly_;
}

template<typename T, typename Y>
Polynomial<T, Y>& Polynomial<T,Y>::operator=(const Polynomial &oth) {
    this->poly_ = oth.poly_;
    return *this;
}

template<typename T, typename Y>
Polynomial<T, Y>::Polynomial(Polynomial &&other) {
    poly_ = other.poly_;
}

template<typename T, typename Y>
Polynomial<T, Y>::Polynomial(const std::vector<Monomial<T>> &oth) {
    poly_ = {};
    for (auto i: oth) {
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
    sort(poly_.begin(), poly_.end(), Y());
    std::merge(poly_.begin(), poly_.end(), other.poly_.begin(), other.poly_.end(), std::back_inserter(monomials),
               Y());
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
        while (tmp.size() && tmp.back().getCoefficient() == 0) {
            tmp.pop_back();
        }
    }
    poly_ = tmp;
    sort(poly_.begin(), poly_.end(), Y());
    return *this;
}

template<typename T, typename Y>
Polynomial<T, Y> &Polynomial<T, Y>::operator-=(const Polynomial &other) {
    Polynomial minus_other = other;
    sort(poly_.begin(), poly_.end(), Y());
    for (auto &monomial: minus_other.poly_) {
        monomial.addCoefficient(monomial.getCoefficient() * -2);
    }
    std::vector<Monomial<T>> monomials;
    std::merge(poly_.begin(), poly_.end(), minus_other.poly_.begin(), minus_other.poly_.end(),
               std::back_inserter(monomials), Y());
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
        while (tmp.size() && tmp.back().getCoefficient() == 0) {
            tmp.pop_back();
        }
    }
    poly_ = tmp;
    sort(poly_.begin(), poly_.end(), Y());
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
Polynomial<T, Y> &Polynomial<T, Y>::operator*=(const Polynomial &other) {
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
        while (tmp.size() && tmp.back().getCoefficient() == 0) {
            tmp.pop_back();
        }
    }
    this->poly_ = tmp;
    sort(poly_.begin(), poly_.end(), Y());
    return *this;
}

template<typename T, typename Y>
Polynomial<T, Y> &Polynomial<T, Y>::operator*=(const Monomial<T> &oth) {
    for (auto &i: poly_) {
        i *= oth;
    }
    return *this;
}

template<typename T, typename Y>
Polynomial<T, Y> Polynomial<T, Y>::operator*(const Monomial<T> &oth) {
    auto res = *this;
    res *= oth;
    return *this;
}

template<typename T, typename Y>
bool Polynomial<T, Y>::operator<(const Polynomial &other) const {
    size_t minlen = std::min(other.poly_.size(), poly_.size());
    for (size_t i = 0; i < minlen; ++i) {
        if (other.poly_[other.poly_.size() - i - 1] == poly_[poly_.size() - i - 1]) {
            continue;
        }
        if (Y()(other.poly_[other.poly_.size() - i - 1], poly_[poly_.size() - i - 1])) {
            return 0;
        }
        return 1;
    }
    return poly_.size() < other.poly_.size();
}

#endif // POLYNOMIAL_H