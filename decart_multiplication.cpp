#include <array>
#include <tuple>
#include <utility>
#include <cstddef>
#include <iostream>

template <typename Tuple1, typename Tuple2, std::size_t... I1, std::size_t... I2>
auto tuple_cat_constexpr_impl(const Tuple1 &t1, const Tuple2 &t2, std::index_sequence<I1...>, std::index_sequence<I2...>)
{
    return std::make_tuple(std::get<I1>(t1)..., std::get<I2>(t2)...);
}

template <typename Tuple1, typename Tuple2>
auto tuple_cat_constexpr(const Tuple1 &t1, const Tuple2 &t2)
{
    return tuple_cat_constexpr_impl(
        t1, t2,
        std::make_index_sequence<std::tuple_size<Tuple1>::value>{},
        std::make_index_sequence<std::tuple_size<Tuple2>::value>{});
}

template <typename T, std::size_t N>
auto cartesian_product(const std::array<T, N> &arr)
{
    std::array<std::tuple<T>, N> result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result[i] = std::make_tuple(arr[i]);
    }
    return result;
}

template <typename T1, std::size_t N1, typename... RestArrays>
auto cartesian_product(const std::array<T1, N1> &arr1, const RestArrays &...rest)
{
    auto rest_result = cartesian_product(rest...);
    constexpr std::size_t N_rest = std::tuple_size<decltype(rest_result)>::value;

    using TupleType = decltype(tuple_cat_constexpr(
        std::make_tuple(arr1[0]),
        rest_result[0]));

    std::array<TupleType, N1 * N_rest> result{};

    for (std::size_t i = 0; i < N1; ++i)
    {
        for (std::size_t j = 0; j < N_rest; ++j)
        {
            result[i * N_rest + j] = tuple_cat_constexpr(
                std::make_tuple(arr1[i]),
                rest_result[j]);
        }
    }

    return result;
};
template <typename Tuple, std::size_t... I>
void print_tuple(const Tuple &t, std::index_sequence<I...>)
{
    using swallow = int[];
    (void)swallow{0, ((std::cout << std::get<I>(t) << " "), 0)...};
}

template <typename... Ts>
void print(const std::tuple<Ts...> &t)
{
    print_tuple(t, std::index_sequence_for<Ts...>{});
    std::cout << '\n';
}

int main()
{
    constexpr std::array<int, 2> a1{{1, 2}};
    constexpr std::array<double, 2> a2{{3.5, 4.5}};
    constexpr std::array<char, 2> a3{{'a', 'b'}};

    auto result = cartesian_product(a1, a2, a3);

    for (const auto &tup : result)
    {
        print(tup);
    }
}
