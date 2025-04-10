#include <iostream>
#include <type_traits>
#include <vector>
#include <list>
#include <array>

// Проверка наличия метода size()
template <typename T, typename = void>
struct has_size : std::false_type
{
};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type
{
};

// Проверка наличия operator[]
template <typename T, typename = void>
struct has_subscript : std::false_type
{
};

template <typename T>
struct has_subscript<T, std::void_t<decltype(std::declval<T>()[0])>> : std::true_type
{
};

// Проверка, что контейнер подходит для адаптера и сортировки
template <typename Container>
constexpr bool is_sortable_container = has_size<Container>::value && has_subscript<Container>::value;

// Адаптер контейнера
template <typename Container>
class ContainerAdapter
{
    static_assert(is_sortable_container<Container>,
                  "Container must have size() and operator[]");

    Container data;

public:
    explicit ContainerAdapter(const Container &cont) : data(cont) {}

    void print() const
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }
};

// Сортировка контейнера
template <typename Container>
void sort_container(Container &cont)
{
    static_assert(is_sortable_container<Container>,
                  "Container must have size() and operator[] to be sortable");

    // Простейшая сортировка пузырьком (для демонстрации)
    for (size_t i = 0; i < cont.size(); ++i)
    {
        for (size_t j = i + 1; j < cont.size(); ++j)
        {
            if (cont[i] > cont[j])
            {
                std::swap(cont[i], cont[j]);
            }
        }
    }
}

int main()
{
    // Работает: std::vector и std::array поддерживают size() и operator[]
    std::vector<int> vec = {5, 3, 1, 4, 2};
    ContainerAdapter<std::vector<int>> adapter_vec(vec); // Явное указание шаблонного аргумента
    adapter_vec.print();                                 // 5 3 1 4 2

    sort_container(vec);
    adapter_vec.print(); // 1 2 3 4 5

    std::array<int, 5> arr = {5, 3, 1, 4, 2};
    ContainerAdapter<std::array<int, 5>> adapter_arr(arr); // Явное указание шаблонного аргумента
    adapter_arr.print();                                   // 5 3 1 4 2

    sort_container(arr);
    adapter_arr.print(); // 1 2 3 4 5

    // Не работает: std::list не поддерживает operator[]
    // std::list<int> lst = {5, 3, 1, 4, 2};
    // ContainerAdapter<std::list<int>> adapter_lst(lst); // Ошибка компиляции
    // sort_container(lst); // Ошибка компиляции

    return 0;
}