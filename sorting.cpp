#include <algorithm>
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <iostream>
#include <chrono>
#include <functional>
#include <random>

namespace my {

template <typename RandomAccessIterator, typename Compare>
void merge(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp) {
    using ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;
    std::vector<ValueType> left(first, middle);
    std::vector<ValueType> right(middle, last);

    auto left_it = left.begin();
    auto right_it = right.begin();
    auto dest_it = first;

    while (left_it != left.end() && right_it != right.end()) {
        if (comp(*left_it, *right_it)) {
            *dest_it = *left_it;
            ++left_it;
        } else {
            *dest_it = *right_it;
            ++right_it;
        }
        ++dest_it;
    }

    std::copy(left_it, left.end(), dest_it);
    std::copy(right_it, right.end(), dest_it);
}

template <typename RandomAccessIterator, typename Compare>
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    auto size = std::distance(first, last);
    if (size <= 1) return;

    auto middle = first + size / 2;
    my::sort(first, middle, comp);
    my::sort(middle, last, comp);
    merge(first, middle, last, comp);
}

}

// Демонстрация работы с разными контейнерами и компараторами
void demonstrate_sort() {
    // 1. Сортировка вектора с обычной функцией
    std::vector<int> vec = {5, 3, 1, 4, 2};
    bool (*func_ptr)(int, int) = [](int a, int b) { return a < b; };
    my::sort(vec.begin(), vec.end(), func_ptr);
    std::cout << "Sorted vector: ";
    for (int x : vec) std::cout << x << " ";
    std::cout << "\n";

    // 2. Сортировка дека с лямбдой
    std::deque<int> deq = {5, 3, 1, 4, 2};
    my::sort(deq.begin(), deq.end(), [](int a, int b) { return a > b; });
    std::cout << "Sorted deque (descending): ";
    for (int x : deq) std::cout << x << " ";
    std::cout << "\n";

    // 3. Сортировка с функтором
    struct {
        bool operator()(int a, int b) const { return a % 2 < b % 2; } // Сначала чётные
    } custom_comp;
    std::vector<int> vec2 = {1, 2, 3, 4, 5, 6};
    my::sort(vec2.begin(), vec2.end(), custom_comp);
    std::cout << "Sorted with functor: ";
    for (int x : vec2) std::cout << x << " ";
    std::cout << "\n";

    // 4. Попытка сортировки списка (не скомпилируется)
    // std::list<int> lst = {5, 3, 1, 4, 2};
    // my::sort(lst.begin(), lst.end(), func_ptr); // Ошибка: итератор не RandomAccess
}

// Измерение времени сортировки
void measure_sort_performance() {
    const size_t size = 100000;
    std::vector<int> vec(size);
    std::deque<int> deq(size);

    // Заполнение случайными значениями
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    auto rand_num = [&]() { return dis(gen); };

    std::generate(vec.begin(), vec.end(), rand_num);
    std::copy(vec.begin(), vec.end(), std::back_inserter(deq));

    auto comp = [](int a, int b) { return a < b; };

    // Сортировка вектора
    auto start_vec = std::chrono::high_resolution_clock::now();
    my::sort(vec.begin(), vec.end(), comp);
    auto end_vec = std::chrono::high_resolution_clock::now();
    auto duration_vec = std::chrono::duration_cast<std::chrono::milliseconds>(end_vec - start_vec);
    std::cout << "Vector sort time: " << duration_vec.count() << " ms\n";

    // Сортировка дека
    auto start_deq = std::chrono::high_resolution_clock::now();
    my::sort(deq.begin(), deq.end(), comp);
    auto end_deq = std::chrono::high_resolution_clock::now();
    auto duration_deq = std::chrono::duration_cast<std::chrono::milliseconds>(end_deq - start_deq);
    std::cout << "Deque sort time: " << duration_deq.count() << " ms\n";
}

int main() {
    demonstrate_sort();
    measure_sort_performance();
    return 0;
}