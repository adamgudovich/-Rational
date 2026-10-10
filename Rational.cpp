#include "Rational.h"

#include <climits>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace {
long long absolute(long long value) {
    return value < 0 ? -value : value;
}

long long gcd(long long a, long long b) {
    a = absolute(a);
    b = absolute(b);

    while (b != 0) {
        const long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

long long checkedAdd(long long a, long long b) {
    if ((b > 0 && a > LLONG_MAX - b) ||
        (b < 0 && a < LLONG_MIN - b)) {
        throw std::overflow_error("Rational arithmetic overflow");
    }
    return a + b;
}

long long checkedMultiply(long long a, long long b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    if ((a == -1 && b == LLONG_MIN) ||
        (b == -1 && a == LLONG_MIN)) {
        throw std::overflow_error("Rational arithmetic overflow");
    }

    if (a > 0) {
        if ((b > 0 && a > LLONG_MAX / b) ||
            (b < 0 && b < LLONG_MIN / a)) {
            throw std::overflow_error("Rational arithmetic overflow");
        }
    } else {
        if ((b > 0 && a < LLONG_MIN / b) ||
            (b < 0 && a < LLONG_MAX / b)) {
            throw std::overflow_error("Rational arithmetic overflow");
        }
    }
    return a * b;
}
} // namespace

Rational::Rational(int numerator, int denominator)
    : numerator_(0), denominator_(1) {
    normalize(numerator, denominator);
}

int Rational::numerator() const {
    return numerator_;
}

int Rational::denominator() const {
    return denominator_;
}

void Rational::normalize(long long numerator, long long denominator) {
    if (denominator == 0) {
        throw std::invalid_argument("Rational denominator cannot be zero");
    }

    if (numerator == 0) {
        numerator_ = 0;
        denominator_ = 1;
        return;
    }

    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }

    const long long divisor = gcd(numerator, denominator);
    numerator /= divisor;
    denominator /= divisor;

    if (numerator < std::numeric_limits<int>::min() ||
        numerator > std::numeric_limits<int>::max() ||
        denominator > std::numeric_limits<int>::max()) {
        throw std::overflow_error("Rational value does not fit into int");
    }

    numerator_ = static_cast<int>(numerator);
    denominator_ = static_cast<int>(denominator);
}

Rational Rational::operator-() const {
    Rational result;
    result.normalize(-static_cast<long long>(numerator_), denominator_);
    return result;
}

Rational& Rational::operator+=(const Rational& other) {
    const long long commonDivisor = gcd(denominator_, other.denominator_);
    const long long leftFactor = other.denominator_ / commonDivisor;
    const long long rightFactor = denominator_ / commonDivisor;

    const long long leftNumerator =
        checkedMultiply(numerator_, leftFactor);
    const long long rightNumerator =
        checkedMultiply(other.numerator_, rightFactor);
    const long long newNumerator =
        checkedAdd(leftNumerator, rightNumerator);
    const long long newDenominator =
        checkedMultiply(denominator_, leftFactor);

    normalize(newNumerator, newDenominator);
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    return *this += -other;
}

Rational& Rational::operator*=(const Rational& other) {
    const long long firstDivisor = gcd(numerator_, other.denominator_);
    const long long secondDivisor = gcd(other.numerator_, denominator_);

    const long long leftNumerator = numerator_ / firstDivisor;
    const long long rightNumerator = other.numerator_ / secondDivisor;
    const long long leftDenominator = denominator_ / secondDivisor;
    const long long rightDenominator = other.denominator_ / firstDivisor;

    normalize(checkedMultiply(leftNumerator, rightNumerator),
              checkedMultiply(leftDenominator, rightDenominator));
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    if (other.numerator_ == 0) {
        throw std::domain_error("Division by zero Rational");
    }

    const long long firstDivisor = gcd(numerator_, other.numerator_);
    const long long secondDivisor = gcd(other.denominator_, denominator_);

    const long long leftNumerator = numerator_ / firstDivisor;
    const long long rightNumerator = other.denominator_ / secondDivisor;
    const long long leftDenominator = denominator_ / secondDivisor;
    const long long rightDenominator = other.numerator_ / firstDivisor;

    normalize(checkedMultiply(leftNumerator, rightNumerator),
              checkedMultiply(leftDenominator, rightDenominator));
    return *this;
}

Rational::operator int() const {
    return numerator_ / denominator_;
}

Rational::operator double() const {
    return static_cast<double>(numerator_) / denominator_;
}

Rational operator+(Rational lhs, const Rational& rhs) {
    lhs += rhs;
    return lhs;
}

Rational operator-(Rational lhs, const Rational& rhs) {
    lhs -= rhs;
    return lhs;
}

Rational operator*(Rational lhs, const Rational& rhs) {
    lhs *= rhs;
    return lhs;
}

Rational operator/(Rational lhs, const Rational& rhs) {
    lhs /= rhs;
    return lhs;
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.numerator_ == rhs.numerator_ &&
           lhs.denominator_ == rhs.denominator_;
}

bool operator!=(const Rational& lhs, const Rational& rhs) {
    return !(lhs == rhs);
}

bool operator<(const Rational& lhs, const Rational& rhs) {
    return static_cast<long long>(lhs.numerator_) * rhs.denominator_ <
           static_cast<long long>(rhs.numerator_) * lhs.denominator_;
}

bool operator<=(const Rational& lhs, const Rational& rhs) {
    return !(rhs < lhs);
}

bool operator>(const Rational& lhs, const Rational& rhs) {
    return rhs < lhs;
}

bool operator>=(const Rational& lhs, const Rational& rhs) {
    return !(lhs < rhs);
}

std::istream& operator>>(std::istream& input, Rational& value) {
    long long numerator = 0;
    long long denominator = 0;

    if (!(input >> numerator)) {
        return input;
    }

    input >> std::ws;
    if (input.peek() == '/') {
        input.get();
    }

    if (!(input >> denominator)) {
        return input;
    }

    if (numerator < std::numeric_limits<int>::min() ||
        numerator > std::numeric_limits<int>::max() ||
        denominator < std::numeric_limits<int>::min() ||
        denominator > std::numeric_limits<int>::max() ||
        denominator == 0) {
        input.setstate(std::ios::failbit);
        return input;
    }

    try {
        Rational temporary(static_cast<int>(numerator),
                           static_cast<int>(denominator));
        value = temporary;
    } catch (const std::exception&) {
        input.setstate(std::ios::failbit);
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const Rational& value) {
    return output << value.numerator_ << '/' << value.denominator_;
}
