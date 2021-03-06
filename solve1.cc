/*
 * Solution to Express puzzle at:
 *     https://fivethirtyeight.com/features/can-you-escape-the-desert/
 *
 * By J. Eric Ivancich
 *
 * See file "License" for license.
 */

#include <stdlib.h>
#include <time.h>

#include <iostream>

#include <boost/rational.hpp>


using Num = unsigned long long;
using Rat = boost::rational<Num>;


Num factorial(Num n) {
    if (n <= 1) return 1;
    Num product = 1;
    for (int i = 2; i <= n; ++i) {
        product *= i;
    }
    return product;
}

// combinatorial calculation of choosing "count" items from "from" items.
Num choose(int from, int count) {
    return factorial(from) / factorial(count) / factorial(from - count);
}

Rat calc(int n, bool show_intermediate = false) {
    Rat sum(0);
    for (int i = 0; i <= n; ++i) {
        Num numerator = choose(n, i);
        Num two_n = 1 << n;
        Num f1 = factorial(i);
        Num f2 = factorial(n - i);
        Num denominator = two_n * f1 * f2;
        if (show_intermediate) {
            std::cout << "num: " << numerator << std::endl;
            std::cout << "denom: " << two_n << " * " << f1 << " * " << f2 << " = " << denominator << std::endl;
        }
        sum += Rat(numerator, denominator);
    }
    return sum;
}

Rat simulate(int n, int iterations) {
    int good = 0;
    int bad = 0;

    while (iterations-- > 0) {
        // track the travlers who go "left" and "right" separately
        int left = 0;
        int right = 0;

        // divide the travelers randomly into "left" and "right" groups
        for (int i = 0; i < n; ++i) {
            if (rand() % 2) {
                left++;
            } else {
                right++;
            }
        }

        bool is_good = true;

        // see if the "left" travelers don't have crossing paths
        int current = 0;
        while (left-- > 0) {
            int r = rand();
            if (r >= current) {
                current = r;
            } else {
                is_good = false;
                break;
            }
        }

        // if we're good at this point, check the "right" travelers
        if (is_good) {
            int current = 0;
            while (right-- > 0) {
                int r = rand();
                if (r >= current) {
                    current = r;
                } else {
                    is_good = false;
                    break;
                }
            }
        }

        if (is_good) ++good;
        else ++bad;
    }

    return Rat(good, good + bad);
}

int main(int argc, char* argv[]) {
    srand (time(NULL));

    // parse command-line
    
    int n = 4;
    if (argc >= 2) {
        n = atoi(argv[1]);
    }

    int iterations = 1000000;
    if (argc >= 3) {
        iterations = atoi(argv[2]);
    }

    const bool show_intermediate = argc >= 4;

    // calculate odds

    const Rat result1 = calc(n, show_intermediate);

    const double d1 = boost::rational_cast<double>(result1);
    std::cout << n << ": " << result1 << " (" << d1 << ")" << std::endl;

    // simulate odds

    Rat result2 = simulate(n, iterations);

    double d2 = boost::rational_cast<double>(result2);
    std::cout << n << ": " << result2 << " (" << d2 << ")" << std::endl;
}
