#include <deque>
#include <string>
#include <stdexcept>
#include <iostream>

template <typename T, typename Container = std::deque<T>>
class Stack {
private:
    Container data;

public:
    void push(const T& value) {
        data.push_back(value);
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        data.pop_back();
    }

    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data.back();
    }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data.back();
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

template <>
class Stack<char, std::string> {
private:
    std::string data;

public:
    void push(char c) {
        data.push_back(c);
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        data.pop_back();
    }

    char top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data.back();
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

int main() {
    // Обычный стек (на std::deque<int>)
    Stack<int> intStack;
    intStack.push(1);
    intStack.push(2);
    std::cout << "Top of intStack: " << intStack.top() << "\n"; // 2
    intStack.pop();
    std::cout << "Top after pop: " << intStack.top() << "\n";   // 1

    // Стек char (специализация для std::string)
    Stack<char, std::string> charStack;
    charStack.push('a');
    charStack.push('b');
    std::cout << "Top of charStack: " << charStack.top() << "\n"; // 'b'
    charStack.pop();
    std::cout << "Top after pop: " << charStack.top() << "\n";     // 'a'

    return 0;
}