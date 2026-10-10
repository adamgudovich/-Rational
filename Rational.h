#ifndef RATIONAL_H
#define RATIONAL_H

#include <iosfwd>

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1);

    int numerator() const;
    int denominator() const;

    Rational operator-() const;

    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);

    explicit operator int() const;
    explicit operator double() const;

    friend bool operator==(const Rational& lhs, const Rational& rhs);
    friend bool operator!=(const Rational& lhs, const Rational& rhs);
    friend bool operator<(const Rational& lhs, const Rational& rhs);
    friend bool operator<=(const Rational& lhs, const Rational& rhs);
    friend bool operator>(const Rational& lhs, const Rational& rhs);
    friend bool operator>=(const Rational& lhs, const Rational& rhs);

    friend std::istream& operator>>(std::istream& input, Rational& value);
    friend std::ostream& operator<<(std::ostream& output, const Rational& value);

private:
    int numerator_;
    int denominator_;

    void normalize(long long numerator, long long denominator);
};

Rational operator+(Rational lhs, const Rational& rhs);
Rational operator-(Rational lhs, const Rational& rhs);
Rational operator*(Rational lhs, const Rational& rhs);
Rational operator/(Rational lhs, const Rational& rhs);

#endif
