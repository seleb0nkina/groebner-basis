#include "algos.h"
#include "deglex.h"
#include "lex.h"
#include "revlex.h"
#include "polynomial.h"
#include "polynomialset.h"
#include <boost/rational.hpp>

int main() {
    Monomial<boost::rational<int>> m(1, {{0, 1},
                                         {1, 2}});

    std::cout << m << std::endl; // ожидается 1x_0^1x_1^2

    Monomial<boost::rational<int>> a(1, {{0, 1},
                                         {1, 1}});
    Monomial<boost::rational<int>> b(7, {{2, 9}});
    std::cout << a << ' ' << b << std::endl;//ожидается 1x_0^1x_1^1 7x_2^9
    std::cout << a * b << std::endl;// ожидается 7x_0^1x_1^1x_2^9
    a *= b;
    std::cout << a << std::endl;// ожидается 7x_0^1x_1^1x_2^9
    Monomial<boost::rational<int>> c(9, {{3, 1}});
    a /= b;
    std::cout << a << std::endl;// ожидается 1x_0^1x_1^1
    Monomial<boost::rational<int>> d(4, {});
    Monomial<boost::rational<int>> e(6, {});
    std::cout << d / e << std::endl;
    std::vector<Monomial<boost::rational<int>>> lol = {a, b, d, e};
    std::sort(lol.begin(), lol.end(), DegLex<boost::rational<int>>());
    for (const auto &i: lol) std::cout << i << ' ';
    std::sort(lol.begin(), lol.end(), Lex<boost::rational<int>>());
    for (const auto &i: lol) std::cout << i << ' ';
   // std::sort(lol.begin(), lol.end(), RevLex<Monomial<boost::rational<int>>>());
    for (const auto &i: lol) std::cout << i << ' ';
    Polynomial<boost::rational<int>, DegLex<boost::rational<int>>> polykek;
    polykek.addMonomial(m);
    //std::cout<<std::endl;
    //std::cout<<polykek<<std::endl;
    PolynomialSet<boost::rational<int>, DegLex<boost::rational<int>>> tset;
    std::cout<<"SET TEST"<<std::endl;
    tset.insert(polykek);
    polykek.addMonomial(d);
    tset.insert(polykek);
    for (auto w : tset) {
        std::cout << w << std::endl;
    }
    std::cout<<std::endl;
    tset.insert(std::vector<Monomial<boost::rational<int>>>{d});
    for (auto w : tset) {
        std::cout << w << std::endl;
    }
    return 0;
}
