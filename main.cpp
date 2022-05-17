#include "deglex.h"
#include "polynomial.h"
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
    std::sort(lol.begin(), lol.end(), DegLex<Monomial<boost::rational<int>>>());
    for (const auto &i: lol) std::cout << i << ' ';
    return 0;
}
