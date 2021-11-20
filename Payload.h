/**
    Justin Hardy
    JEH180008
*/

#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <iostream>
#include <ostream>
#include <sstream>
#include <cmath>

class Payload {
private:
    /// Private Instance Variables
    int numerator, denominator;
    int exponent;

    /// Private Functions
    int GetGCD(int, int);
public:
    /// Constructors
    Payload(), Payload(int, int), Payload(int, int, int);

    virtual ~Payload() {};

    /// Getters
    int GetNumerator();
    int GetDenominator();
    std::string GetCoefficient();
    double GetCoefficientAsDouble();
    int GetExponent();

    /// Setters
    void SetNumerator(int);
    void SetDenominator(int);
    void SetExponent(int);
    void SimplifyCoefficient();

    /// Public Functions
    double Evaluate(int);

    /// Overrides
    Payload& operator+(Payload&);
    void operator+=(Payload&);
    bool operator<(Payload&);
    bool operator>(Payload&);
    bool operator==(Payload&);
    friend std::ostream& operator<<(std::ostream&, Payload&);
};

#endif // PAYLOAD_H
