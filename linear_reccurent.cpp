#include <iostream>
#include <array> // Необходим для std::array

template<size_t N, typename T = int>
constexpr T linear_recurrence(const std::array<T, N>& coefficients,
                             const std::array<T, N>& initial,
                             size_t index) {
    if (index < N) {
        return initial[index];
    }

    T result = 0;
    for (size_t i = 0; i < N; ++i) {
        result += coefficients[i] * linear_recurrence<N>(coefficients, initial, index - 1 - i);
    }
    return result;
}

int main() {
    // Пример с числами Фибоначчи (рекуррент длины 2)
    constexpr std::array<int, 2> fib_coeffs = {1, 1};
    constexpr std::array<int, 2> fib_init = {0, 1};

    constexpr int fib10 = linear_recurrence<2>(fib_coeffs, fib_init, 10);
    std::cout << "Fibonacci(10) = " << fib10 << std::endl;

    // Пример с рекуррентом длины 3
    constexpr std::array<int, 3> coeffs = {1, 1, 1};
    constexpr std::array<int, 3> init = {0, 1, 2};

    constexpr int elem7 = linear_recurrence<3>(coeffs, init, 7);
    std::cout << "Element 7 = " << elem7 << std::endl;

    return 0;
}