#include <iostream>
#include <tuple>
#include <utility>
#include <type_traits>

// Базовый случай рекурсии (пустой кортеж)
template <size_t Index = 0, typename Func, typename... Args>
inline std::enable_if_t<Index == sizeof...(Args), void>
apply_to_tuple(const std::tuple<Args...>&, Func&&) {}

// Рекурсивный случай для константного кортежа (только чтение)
template <size_t Index = 0, typename Func, typename... Args>
inline std::enable_if_t<Index < sizeof...(Args), void>
apply_to_tuple(const std::tuple<Args...>& tuple, Func&& func) {
    func(std::get<Index>(tuple)); // Применяем операцию (только чтение)
    apply_to_tuple<Index + 1>(tuple, std::forward<Func>(func));
}

// Перегрузка для неконстантного кортежа (можно изменять элементы)
template <size_t Index = 0, typename Func, typename... Args>
inline std::enable_if_t<Index < sizeof...(Args), void>
apply_to_tuple(std::tuple<Args...>& tuple, Func&& func) {
    func(std::get<Index>(tuple)); // Можно изменять элементы
    apply_to_tuple<Index + 1>(tuple, std::forward<Func>(func));
}

int main() {
    // Пример с константным кортежом (только чтение)
    const auto const_tuple = std::make_tuple(1, 3.14, "Hello");
    std::cout << "Const tuple: ";
    apply_to_tuple(const_tuple, [](const auto& x) {
        std::cout << x << " ";
    });
    std::cout << "\n";

    // Пример с изменяемым кортежом
    auto mutable_tuple = std::make_tuple(5, 10.5, 20);
    apply_to_tuple(mutable_tuple, [](auto& x) {
        if constexpr (std::is_arithmetic_v<std::decay_t<decltype(x)>>) {
            x += 1; // Теперь можно изменять!
        }
    });

    std::cout << "Modified tuple: ";
    apply_to_tuple(mutable_tuple, [](const auto& x) {
        std::cout << x << " ";
    });
    std::cout << "\n";

    return 0;
}