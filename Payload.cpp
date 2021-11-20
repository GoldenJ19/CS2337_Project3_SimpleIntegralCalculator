/**
    Justin Hardy
    JEH180008
*/

#include "Payload.h"

/// Constructors
// Creates a default payload. (0)
Payload::Payload() {
    numerator = 0;
    denominator = 1;
    exponent = 0;
}

// Creates a payload with a given numerator and exponent.
Payload::Payload(int n, int e) {
    numerator = n;
    denominator = 1;
    exponent = e;
}

// Creates a payload with a given numerator, denominator, and exponent.
Payload::Payload(int n, int d, int e) {
    numerator = n;
    denominator = d;
    exponent = e;

    // Simplify Coefficient
    SimplifyCoefficient();
}

/// Private Functions
// Returns the greatest common denominator of given 2 numbers.
int Payload::GetGCD( int a, int b ) {
    if( a == 0 ) {
        return b;
    }
    else if( b == 0 ) {
        return a;
    }
    return a < b ? GetGCD(a, b%a) : GetGCD(b, a%b);
}

/// Getters
// Returns a string-representation of the coefficient of the numerator and denominator.
// May be returned as either a fraction or whole number. Will never return decimals.
std::string Payload::GetCoefficient() {
    std::stringstream stream;
    if(denominator != 0) {
        if( std::abs(denominator) != 1 ) {
            // The Coefficient must be written as a fraction.
            stream << "(" << std::abs(numerator) << "/" << std::abs(denominator) << ")";
        }
        else if( std::abs(numerator) != 1 || exponent == 0 )
        {
            // The coefficient can be written as a whole number.
            stream << std::abs(numerator/denominator);
        }
        else {
            // The coefficient is invalid in some way...
            return "";
        }
    }
    else {
        // The coefficient is undefined. (n/0)
        stream << "UND";
    }
    return stream.str();
}

// Returns a double-representation of the coefficient of the numerator and denominator.
double Payload::GetCoefficientAsDouble() {
    return (double)numerator / denominator;
}

// Returns the numerator.
int Payload::GetNumerator() {
    return numerator;
}

// Returns the denominator.
int Payload::GetDenominator() {
    return denominator;
}

// Returns the exponent.
int Payload::GetExponent() {
    return exponent;
}

/// Setters
// Sets the numerator to a given value.
void Payload::SetNumerator( int value ) {
    numerator = value;
    SimplifyCoefficient();
}

// Sets the denominator to a given value.
void Payload::SetDenominator( int value ) {
    denominator = value;
    SimplifyCoefficient();
}

// Sets the exponent to a given value.
void Payload::SetExponent( int value ) {
    exponent = value;
}

// Simplifies the numerator and denominator by their greatest common denominator.
void Payload::SimplifyCoefficient() {
    int GCD = GetGCD(std::abs(numerator), std::abs(denominator));
    numerator /= GCD;
    denominator /= GCD;
}

/// Public Functions
// Returns an evaluation of the payload with a given x-value.
double Payload::Evaluate( int x ) {
    return GetCoefficientAsDouble() * std::pow(x, exponent);
}

/// Overloads
// No comments needed for these, very self explanatory.
Payload& Payload::operator+( Payload& other ) {
    if( exponent == other.exponent ) {
        int denom = denominator * other.denominator;
        int thisNum = numerator * other.denominator;
        int otherNum = other.numerator * denominator;
        Payload payload(thisNum+otherNum, denom, exponent);
        return payload;
    }
}

void Payload::operator+=( Payload& other ) {
    if( exponent == other.exponent ) {
        int denom = denominator * other.denominator;
        int thisNum = numerator * other.denominator;
        int otherNum = other.numerator * denominator;
        numerator = thisNum+otherNum;
        denominator = denom;
        SimplifyCoefficient();
    }
}

bool Payload::operator<( Payload& other ) {
    return exponent < other.exponent;
}

bool Payload::operator>( Payload& other ) {
    return exponent > other.exponent;
}

bool Payload::operator==( Payload& other ) {
    return exponent == other.exponent;
}

// Returns a formatted string of the payload.
std::ostream& operator<<( std::ostream& stream, Payload& payload ) {
    if(payload.numerator != 0 ) {
        if( (double)payload.numerator/payload.denominator < 0 ) {
            stream << "- ";
        }
        else if( (double)payload.numerator/payload.denominator > 0 ) {
            stream << "+ ";
        }
        stream << payload.GetCoefficient();

        if( payload.exponent != 0 ) {
            stream << "x";
            if( payload.exponent > 1 || payload.exponent < 0 ) {
                stream << "^" << payload.exponent;
            }
        }
    }
    return stream;
}
