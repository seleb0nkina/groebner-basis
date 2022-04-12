#include "main.h"

template<class T>
Monom<T> Monom<T>::operator=(Monom b) {
	coeff = b.coeff;
	powers = b.powers;
	return *this;
}
template<class T>
Monom<T>::Monom() {
	coeff = 0;
	powers.clear();
}

template<class T>
Monom<T>::Monom<T>(std::string expr) {
	coeff = 0;
	size_t fp = 0;
	while (fp < expr.size() && expr[fp] >= '0' && expr[fp] <= '9') {
		++fp;
	}
	if (fp == 0) {
		coeff = 1;
	}
	else {
		coeff = std::stoi(std::string(expr.begin(), expr.begin() + fp));
	}
	for (int j = fp; j < expr.size(); ++j) {
		std::string name;
		int ti = j;
		while (ti < expr.size() && expr[ti] != '^') {
			name += expr[ti];
			++ti;
		}
		int power = 0;
		++ti;
		while (ti < expr.size() && expr[ti] >= '0' && expr[ti] <= '9') {
			power *= 10;
			power += expr[ti] - '0';
			++ti;
		}
		j = ti - 1;
		powers[name] = power;
	}
}

template<class T>
Monom<T> Monom<T>::operator*(Monom b) {
	Monom temp = *this;
	temp.coeff *= b.coeff;
	for (auto j : b.powers) {
		temp.powers[j.first] += j.second;
	}
	return temp;
}

template<class T>
Monom<T> Monom<T>::operator/(Monom b) {
	Monom temp = *this;
	temp.coeff /= b.coeff;
	for (auto j : b.powers) {
		temp.powers[j.first] -= j.second;
	}
	return temp;
}

template<class T>
Monom<T> Monom<T>::operator*=(Monom b) {
	coeff *= b.coeff;
	for (auto j : b.powers) powers[j.first] += j.second;
	return *this;
}

template<class T>
Monom<T> Monom<T>::operator/=(Monom b) {
	coeff /= b.coeff;
	for (auto j : b.powers) powers[j.first] -= j.second;
	return *this;
}

template<class T> 
void Monom<T>::print() {
	std::cout << coeff;
	for (auto j : powers) {
		std::cout << j.first << '^' << j.second;
	}
}
