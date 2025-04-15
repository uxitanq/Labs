#include <iostream>
#include <cmath>
#include <iomanip>

int main() {
    double sum = 0.0;
    double pi_approx;
    const double exact_pi = acos(-1);
    int iterations = 0;
    int k = 1;
    std::cout << std::setprecision(15);
    double eight = 8.0;
    int k_term;

    for (; ;) {
        __asm {
            mov eax, k
            add eax, eax
            dec eax
            mov k_term, eax

            fild k_term
            fld st(0)
            fmulp st(1), st(0)
            fld1
            fdivrp st(1), st(0)

            fld sum
            faddp st(1), st(0)
            fstp sum

            fld sum
            fmul eight
            fsqrt
            fstp pi_approx

            mov eax, iterations
            inc eax
            mov iterations, eax

            mov eax, k
            inc eax
            mov k, eax
        }
        /**if (iterations % 100000 == 0) {
            std::cout << "Iterations: " << iterations << ", approximation of pi: " << pi_approx << "\n";
        } **/

            if (fabs(pi_approx - exact_pi) < 0.00000001) {
                break;
            }
    }

    std::cout << "approximate pi: " << pi_approx << "\n";
    std::cout << "exact pi:      " << exact_pi << "\n";
    std::cout << "Number of iterations:    " << iterations << "\n";

    return 0;
}