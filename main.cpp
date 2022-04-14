#include "monomial.h"

int main() {
    Monomial<int> m(1, {{0, 1},
                        {1, 2}});

    std::cout << m << std::endl; // ожидается 1x_0^1x_1^2

    Monomial<int> a(1, {{0, 1},
                        {1, 1}});
    Monomial<int> b(7, {{2, 9}});
    std::cout << a << ' ' << b << std::endl;//ожидается 1x_0^1x_1^1 7x_2^9
    std::cout << a * b << std::endl;// ожидается 7x_0^1x_1^1x_2^9
    a *= b;
    std::cout << a << std::endl;// ожидается 7x_0^1x_1^1x_2^9
    Monomial<int> c(9, {{3, 1}});
    a /= b;
    std::cout << a << std::endl;// ожидается 1x_0^1x_1^1

    return 0;
}
