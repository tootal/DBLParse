#pragma once

#include <QString>

class BigInteger
{
public:
    static const int BASE = 10;
    //@{
    /**
     * BigInteger constructor
     * @param number - The initial value of the BigInteger
     */
    BigInteger(QString number);
    BigInteger(long long number = 0);
    //@}

    /**
     * Add another BigInteger to the current instance
     * @param other - The other BigInteger
     * @return The sum of the two BigIntegers
     */
    BigInteger add(const BigInteger &other);

    /**
     * Subtract another BigInteger from the current instance
     * @param other - The other BigInteger
     * @return The difference of the two BigIntegers
     */
    BigInteger subtract(const BigInteger &other);

    /**
     * Multiply the current instance by another BigInteger
     * @param other - The other BigInteger
     * @return The product of the two BigIntegers
     */
    BigInteger multiply(BigInteger other);
    
    /**
     * Divide the current instance by another BigInteger
     * @param other - The other BigInteger
     * @return The quotient of the two BigIntegers
     */
    BigInteger divide(BigInteger other);

    /**
     * Raise the current instance to the power of an exponent
     * @param exponent - The power to be raised by
     * @return - The resulting BigInteger after exponentiation
     */
    BigInteger pow(int exponent);

    /**
     * Get the string value of the current instance
     * @return The BigInteger as a string
     */
    QString getString();

    /**
     * Set the value of the current instance with a string
     * @param newStr - The new value for the BigInteger
     * @return The BigInteger with the new value
     */
    BigInteger setString(const QString &newStr);

    /**
     * Negates the current instance
     * @return The BigInteger after negation
     */
    BigInteger negate();
    
    BigInteger trimLeadingZeros();

    //@{
    /**
     * Check if another BigInteger is equal to the current instance
     * @param other - The other BigInteger
     * @return True if equal, otherwise false
     */
    bool equals(const BigInteger &other);
    bool equals(const long long &other);
    bool equals(const QString &other);
    //@}

    /**
     * Get the number of digits in the current instance
     * @return The number of digits
     */
    size_t digits();

    /**
     * Get whether or not the current instance is a negative number
     * @return True if negative, otherwise false
     */
    bool isNegative() const;

    /**
     * Get whether or not the current instance is a positive number
     * @return True if positive, otherwise false
     */
    bool isPositive();

    /**
     * Get whether or not the current instance is an even number
     * @return True if even, otherwise false
     */
    bool isEven();

    /**
     * Get whether or not the current instance is an odd number
     * @return True if odd, otherwise false
     */
    bool isOdd();

    /**
     * Get the absolute value of the current instance
     * @return The absolute value of the BigInteger
     */
    BigInteger abs() const;
    
    //@{
    /**
     * Addition operator
     * @param b1 - The current instance
     * @param b2 - The number being added
     * @return The sum of the two numbers
     */
    friend BigInteger operator+(BigInteger b1, const BigInteger &b2);
    friend BigInteger operator+(BigInteger b1, const long long &b2);
    friend BigInteger operator+(BigInteger b1, const QString &b2);
    //@}
    
    //@{
    /**
     * Subtraction operator
     * @param b1 - The current instance
     * @param b2 - The number being subtracted
     * @return The difference of the two numbers
     */
    friend BigInteger operator-(BigInteger b1, const BigInteger &b2);
    friend BigInteger operator-(BigInteger b1, const long long &b2);
    friend BigInteger operator-(BigInteger b1, const QString &b2);
    //@}
    
    //@{
    /**
     * Multiplication operator
     * @param b1 - The current instance
     * @param b2 - The number being multiplied by
     * @return The product of the two numbers
     */
    friend BigInteger operator*(BigInteger b1, const BigInteger &b2);
    friend BigInteger operator*(BigInteger b1, const long long &b2);
    friend BigInteger operator*(BigInteger b1, const QString &b2);
    //@}
    
    //@{
    /**
     * Division operator
     * @param b1 - The current instance
     * @param b2 - The number being divided by
     * @return The quotient of the two numbers
     */
    friend BigInteger operator/(BigInteger b1, const BigInteger &b2);
    friend BigInteger operator/(BigInteger b1, const long long &b2);
    friend BigInteger operator/(BigInteger b1, const QString &b2);
    //@}
    
    /**
     * Exponent operator
     * @param b1 - The current instance
     * @param b2 - The exponent
     * @return The value after exponentiation
     */
    friend BigInteger operator^(BigInteger b1, const int &b2);
    
    //@{
    /**
     * Equality operator
     * @param b1 - The current instance
     * @param b2 - Another value
     * @return True if equal, otherwise false
     */
    friend bool operator==(BigInteger b1, const BigInteger &b2);
    friend bool operator==(BigInteger b1, const long long &b2);
    friend bool operator==(BigInteger b1, const QString &b2);
    //@}
    
    /**
     * Greater-than operator
     * @param b1 - The current instance
     * @param b2 - Another BigInteger
     * @return True if current instance is greater, otherwise false
     */
    friend bool operator>(BigInteger b1, const BigInteger &b2);
    
    /**
     * Less-than operator
     * @param b1 - The current instance
     * @param b2 - Another BigInteger
     * @return True if current instance is less, otherwise false
     */
    friend bool operator<(BigInteger b1, const BigInteger &b2);
    
    /**
     * Greater-than or equal-to operator
     * @param b1 - The current instance
     * @param b2 - Another BigInteger
     * @return True if current instance is greater or equal, otherwise false
     */
    friend bool operator>=(BigInteger b1, const BigInteger &b2);
    
    /**
     * Less-than or equal-to operator
     * @param b1 - The current instance
     * @param b2 - Another BigInteger
     * @return True if current instance is less or equal, otherwise false
     */
    friend bool operator<=(BigInteger b1, const BigInteger &b2);

    //@{
    /**
     * Assignment operator
     * @param other - The new value for the BigInteger
     * @return A BigInteger containing the new value
     */
    BigInteger& operator=(const BigInteger &other);
    BigInteger& operator=(const long long &other);
    BigInteger& operator=(const QString &other);
    //@}
    
    //@{
    /**
     * Addition assignment operator\n
     * Adds and assigns a value to the current instance
     * @param other - The value being added
     * @return The new value after addition and assignment
     */
    BigInteger& operator+=(const BigInteger &other);
    BigInteger& operator+=(const long long &other);
    BigInteger& operator+=(const QString &other);
    //@}
    
    //@{
    /**
     * Subtraction assignment operator\n
     * Subtracts and assigns a value to the current instance
     * @param other - The value being subtracted
     * @return The new value after subtraction and assignment
     */
    BigInteger& operator-=(const BigInteger &other);
    BigInteger& operator-=(const long long &other);
    BigInteger& operator-=(const QString &other);
    //@}
    
    //@{
    /**
     * Multiplication assignment operator\n
     * Multiplies and assigns a value to the current instance
     * @param other - The value being multiplied
     * @return The new value after multiplication and assignment
     */
    BigInteger& operator*=(const BigInteger &other);
    BigInteger& operator*=(const long long &other);
    BigInteger& operator*=(const QString &other);
    //@}
    
    //@{
    /**
     * Division assignment operator\n
     * Divides and assigns a value to the current instance
     * @param other - The value being divided
     * @return The new value after division and assignment
     */
    BigInteger& operator/=(const BigInteger &other);
    BigInteger& operator/=(const long long &other);
    BigInteger& operator/=(const QString &other);
    //@}

    /**
     * Pre-increment operator
     * @return The incremented BigInteger
     */
    BigInteger& operator++();
    
    /**
     * Pre-decrement operator
     * @return The decremented BigInteger
     */
    BigInteger& operator--();
    
    /**
     * Post-increment operator
     * @return The incremented BigInteger
     */
    BigInteger operator++(int);
    
    /**
     * Post-decrement operator
     * @return The decremented BigInteger
     */
    BigInteger operator--(int);

    /**
     * The index operator
     * @param index The position being looked at
     * @return The number at the specified position in the BigInteger string
     */
    unsigned int operator[](int index);

private:
    QString _numberString;      //The big number represented as a string

    //Methods
    BigInteger addll(const long long &other);
    BigInteger addstr(const QString &other);
    BigInteger subtractll(const long long &other);
    BigInteger subtractstr(const QString  &other);
    BigInteger multiplyll(const long long &other);
    BigInteger multiplystr(const QString &other);
    BigInteger dividell(const long long &other);
    BigInteger dividestr(const QString &other);
    
    friend QDataStream& operator>>(QDataStream &in, BigInteger &x);
    friend QDataStream& operator<<(QDataStream &out, const BigInteger &x);
};

