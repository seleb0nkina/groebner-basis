#include <iostream>
#include <map>
#include <string>
#include <fstream>
template<class T>
class Monom
{
public:
	T coeff;
	std::map<std::string, int> powers;
	Monom();
	Monom(std::string expr);
	Monom operator*(Monom b);
	Monom operator/(Monom b);
	Monom operator*=(Monom b);
	Monom operator/=(Monom b);
	Monom operator=(Monom b);
	void print();
	~Monom() {}
};
