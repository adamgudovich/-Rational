#include "Rational.h"

#include <exception>
#include <iostream>
#include <sstream>

int main() {
    try {
        Rational a(6, 8); // нормализуется в 3/4
        Rational b(-10, 4); // нормализуется в -5/2

        std::cout << "Normalized values:\n";
        std::cout << "a = " << a << '\n';
        std::cout << "b = " << b << '\n';

        std::cout << "\nArithmetic:\n";
        std::cout << "-a = " << -a << '\n';
        std::cout << "a + b = " << a + b << '\n';
        std::cout << "a - b = " << a - b << '\n';
        std::cout << "a * b = " << a * b << '\n';
        if (b != Rational(0)) {
            std::cout << "a / b = " << a / b << '\n';
        } else {
            std::cout << "a / b is undefined (b is zero)\n";
        }

        Rational compound = a;
        compound += b;
        std::cout << "\nAfter compound += : " << compound << '\n';
        compound -= b;
        std::cout << "After compound -= : " << compound << '\n';
        compound *= b;
        std::cout << "After compound *= : " << compound << '\n';
        if (b != Rational(0)) {
            compound /= b;
            std::cout << "After compound /= : " << compound << '\n';
        }

        std::cout << std::boolalpha;
        std::cout << "\nComparisons:\n";
        std::cout << "a == b: " << (a == b) << '\n';
        std::cout << "a != b: " << (a != b) << '\n';
        std::cout << "a < b: " << (a < b) << '\n';
        std::cout << "a <= b: " << (a <= b) << '\n';
        std::cout << "a > b: " << (a > b) << '\n';
        std::cout << "a >= b: " << (a >= b) << '\n';

        std::cout << "\nConversions of a:\n";
        std::cout << "int: " << static_cast<int>(a) << '\n';
        std::cout << "double: " << static_cast<double>(a) << '\n';

        // Проверка оператора ввода на заранее заданных строках
        std::cout << "\nStream input checks:\n";
        {
            std::istringstream in1("5/10");
            Rational r1;
            if (in1 >> r1) {
                std::cout << "read \"5/10\" -> " << r1 << '\n';
            } else {
                std::cout << "read \"5/10\" -> FAILED\n";
            }
        }
        {
            std::istringstream in2("7/0");
            Rational r2;
            if (in2 >> r2) {
                std::cout << "read \"7/0\" -> " << r2 << '\n';
            } else {
                std::cout << "read \"7/0\" -> FAILED (expected)\n";
            }
        }
        {
            std::istringstream in3("4 8"); // формат без слэша
            Rational r3;
            if (in3 >> r3) {
                std::cout << "read \"4 8\" -> " << r3 << '\n';
            } else {
                std::cout << "read \"4 8\" -> FAILED\n";
            }
        }

        // Проверка обработки ошибок
        std::cout << "\nException checks:\n";
        try {
            Rational bad(1, 0);
            std::cout << "1/0 was constructed (unexpected)\n";
        } catch (const std::exception& e) {
            std::cout << "Rational(1, 0) threw: " << e.what() << '\n';
        }

        try {
            Rational x(1, 2);
            Rational y(0, 1);
            std::cout << "x / y = " << x / y << '\n';
        } catch (const std::exception& e) {
            std::cout << "Division by zero threw: " << e.what() << '\n';
        }

        try {
            Rational big(2147483647, 1); // INT_MAX
            Rational two(2, 1);
            std::cout << "big * two = " << big * two << '\n';
        } catch (const std::exception& e) {
            std::cout << "Overflow threw: " << e.what() << '\n';
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
