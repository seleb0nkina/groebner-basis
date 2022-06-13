#include "algos.h"
#include "deglex.h"
#include "lex.h"
#include "revlex.h"
#include "polynomial.h"
#include "polynomialset.h"
#include <boost/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <ctime>
//#define int long long
using namespace std;

void testCorrectness() {
    {//Monomial + Monomial
        Monomial<boost::rational<int>> a(2, {{0, 1},
                                             {1, 2}});
        Monomial<boost::rational<int>> b(228, {{0, 1},
                                               {1, 2}});
        assert((a + b).fullEqual(Monomial<boost::rational<int>>(230, {{0, 1},
                                                                      {1, 2}})));
    }
    {//Monomial - Monomial
        Monomial<boost::rational<int>> a(20, {{0, 1},
                                              {1, 2}});
        Monomial<boost::rational<int>> b(18, {{0, 1},
                                              {1, 2}});
        assert((a - b).fullEqual(Monomial<boost::rational<int>>(2, {{0, 1},
                                                                    {1, 2}})));
    }
    {//Monomial * Monomial
        Monomial<boost::rational<int>> a(20, {{0, 1},
                                              {1, 2}});
        Monomial<boost::rational<int>> b(4, {{1, 3},
                                             {2, 1}});
        assert((a * b).fullEqual(Monomial<boost::rational<int>>(80, {{0, 1},
                                                                     {1, 5},
                                                                     {2, 1}})));
    }
    {//Monomial / Monomial
        Monomial<boost::rational<int>> a(20, {{0, 1},
                                              {1, 2}});
        Monomial<boost::rational<int>> b(4, {{1, 3},
                                             {2, 1}});
        assert(a.isDivisibleBy(b) == false);
        Monomial<boost::rational<int>> c(10, {{0, 1},
                                              {1, 1}});
        assert((a / c).fullEqual(Monomial<boost::rational<int>>(2, {{1, 1}})));
    }
    {//Monomial order
        Monomial<int> a(20, {{0, 1},
                                              {1, 2}});
        Monomial<int> b(4, {{1, 3},
                                             {2, 1}});
        assert(!(Lex<int>()(a,b)));
        auto c = a;
        assert(!Lex<int>()(a, c));
        assert(!Lex<int>()(c, a));
        assert(DegLex<int>()(a, b));
    }
    {//Polynomial construction
        Polynomial<int, Lex<int>> A;
        A.addMonomial(Monomial<int>(2, {{1, 1}}));
        assert(A.LeadingTerm().fullEqual(Monomial<int>(2, {{1, 1}})));
        A.addMonomial(Monomial<int>(1, {{0, 1}}));
        assert(A.LeadingTerm().fullEqual(Monomial<int>(1, {{0, 1}})));
    }
    {//Polynomial + Polynomial
        Polynomial<int, Lex<int>> A, B;
        A.addMonomial(Monomial<int>(2, {{1, 1}}));
        B.addMonomial(Monomial<int>(1, {{0, 1}}));
        auto C = A + B;
        assert(C.LeadingTerm().fullEqual(Monomial<int>(1, {{0, 1}})));
    }
    {//Polynomial - Polynomial
        Polynomial<int, Lex<int>> A, B;
        A.addMonomial(Monomial<int>(2, {{1, 1}}));
        B.addMonomial(Monomial<int>(1, {{0, 1}}));
        auto C = A - B;
        assert(C.LeadingTerm().fullEqual(Monomial<int>(-1, {{0, 1}})));
        C = A - A;
        assert(C.getSize() == 0);
    }
    {//Polynomial * Polynomial
        Polynomial<int, Lex<int>> A, B;
        A.addMonomial(Monomial<int>(2, {{1, 1}}));
        B.addMonomial(Monomial<int>(1, {{0, 1}}));
        auto C = A * B;
        assert(C.LeadingTerm().fullEqual(Monomial<int>(2, {{0, 1},
                                                           {1, 1}})));
        assert(C.getSize() == 1);
    }
    {//Buchberger
        Monomial<boost::rational<int>> X = Monomial<boost::rational<int>>(7, {{0, 1},
                                                                              {2, 3}}), Y = Monomial<boost::rational<int>>(
                1, {{0, 2}});
        Polynomial<boost::rational<int>, Lex<boost::rational<int>>> A(std::vector<Monomial<boost::rational<int>>>{X, Y});
        Polynomial<boost::rational<int>, Lex<boost::rational<int>>> B(std::vector<Monomial<boost::rational<int>>>{X * Y});
        PolynomialSet<boost::rational<int>, Lex<boost::rational<int>>> S;
        S.insert(A);
        S.insert(B);
        buchberger(S);
        assert((liesInIdeal<boost::rational<int>, Lex<boost::rational<int>>>(A, S)));
    }
}

template<typename TFunction>
class TimerWrapper {
public:
    TimerWrapper(TFunction function, clock_t &elapsedTime) :
            call(function),
            startTime_(::clock()),
            elapsedTime_(elapsedTime) {
    }

    ~TimerWrapper() {
        const clock_t endTime_ = ::clock();
        const clock_t diff = (endTime_ - startTime_);
        elapsedTime_ += diff;
    }

    TFunction call;
private:
    const clock_t startTime_;
    clock_t &elapsedTime_;
};


template<typename TFunction>
TimerWrapper<TFunction> test_time(TFunction function, clock_t &elapsedTime) {
    return TimerWrapper<TFunction>(function, elapsedTime);
}

template<typename T, typename Y>
PolynomialSet<T, Y> getCyclic(int n) {
    PolynomialSet<T, Y> s;
    for (int i = 1; i < n; ++i) {
        Polynomial<T, Y> now;
        for (int j = 0; j < n; ++j) {
            Monomial<T> tmp;
            std::vector<std::pair<int, int>> monomInit;
            for (int x = 0; x < i; ++x) {
                int id = (j + x) % n;
                monomInit.push_back(std::make_pair(id, 1));
            }
            for (int i = 0; i < monomInit.size(); ++i) {
                tmp *= Monomial<T>(T(1), {{monomInit[i].first, monomInit[i].second}});
            }
            now.addMonomial(tmp);
        }
        s.insert(now);
    }
    Monomial<T> tmp;
    for (int i = 0; i < n; ++i) {
        tmp *= Monomial<T>(T(1), {{i, 1}});
    }
    std::vector<Monomial<T>> to_init;
    to_init.push_back(tmp);
    to_init.push_back(Monomial<T>());
    to_init.back().addCoefficient(T(-2));
    s.insert(Polynomial<T, Y>(to_init));
    return s;
}

const int iterations = 20;
const int maxtest = 4;

void testSpeed() {
    //Алгоритм будет тестироваться на многочленах cyclic_n
    {//Тестируем порядок Lex
        for (int n = 1; n <= maxtest; ++n) {
            auto now = getCyclic<boost::rational<boost::multiprecision::cpp_int>, Lex<boost::rational<boost::multiprecision::cpp_int>>>(
                    n);
            clock_t elapsedTime = clock();
            std::cout << "Testing  " << std::to_string(n) << std::endl;
            double mean = 0;
            for (int iter = 0; iter < iterations; ++iter) {
                clock_t telapsed = elapsedTime;
                auto nowcopy = now;
                test_time(
                        buchberger<boost::rational<boost::multiprecision::cpp_int>, Lex<boost::rational<boost::multiprecision::cpp_int>>>,
                        elapsedTime).call(nowcopy);
                std::cout << "Lex order attempt " + std::to_string(iter) + " in " << ' '
                          << (double) (elapsedTime - telapsed) / CLOCKS_PER_SEC << std::endl;
                mean += (double)(elapsedTime - telapsed) / CLOCKS_PER_SEC;
            }
            std::cout<<"Average time: "<<mean/iterations<<std::endl;
        }
    }
    {//Тестируем порядок DegLex
        for (int n = 1; n <= maxtest; ++n) {
            auto now = getCyclic<boost::rational<boost::multiprecision::cpp_int>, DegLex<boost::rational<boost::multiprecision::cpp_int>>>(
                    n);
            clock_t elapsedTime = clock();
            std::cout << "Testing  " << std::to_string(n) << std::endl;
            double mean=0;
            for (int iter = 0; iter < iterations; ++iter) {
                clock_t telapsed = elapsedTime;
                test_time(
                        buchberger<boost::rational<boost::multiprecision::cpp_int>, DegLex<boost::rational<boost::multiprecision::cpp_int>>>,
                        elapsedTime).call(now);
                std::cout << "DegLex order attempt " + std::to_string(iter) + " in " << ' '
                          << (double) (elapsedTime - telapsed) / CLOCKS_PER_SEC << std::endl;
                mean += (double)(elapsedTime - telapsed) / CLOCKS_PER_SEC;
            }
            std::cout<<"Average time: "<<mean / iterations<<std::endl;
        }

    }
}

signed main() {
    Monomial<boost::rational<int>> m(1, {{0, 1},
                                         {1, 2}});

    testCorrectness();
    testSpeed();
    //std::cout << m << std::endl; // ожидается 1x_0^1x_1^2

/*  Monomial<boost::rational<int>> a(1, {{0, 1},
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
    std::cout << "SET TEST" << std::endl;
    //assert(0);
    tset.insert(polykek);
    polykek.addMonomial(d);
    tset.insert(polykek);
    for (auto w: tset) {
        std::cout << w << std::endl;
    }
    std::cout << std::endl;
    tset.insert(std::vector<Monomial<boost::rational<int>>>{d});
    for (auto w: tset) {
        std::cout << w << std::endl;
    }*/
    return 0;
}