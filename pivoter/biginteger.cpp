#include "biginteger.h"

#include <QDebug>

BigInteger::BigInteger(QString number) :
        _numberString(number)
{
}

BigInteger::BigInteger(long long number) :
    _numberString(QString::number(number))
{}

BigInteger BigInteger::add(const BigInteger &other) {
    BigInteger b1 = other > *this ? other : *this;
    BigInteger b2 = other > *this ? *this : other;
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            return b1.negate().add(b2.negate()).negate();
        }
        if (b1.isNegative() && !b2.isNegative()) {
            return b1.negate().subtract(b2).negate();
        }
        return b2.negate().subtract(b1).negate();
    }
    QString results;
    int carry = 0;
    int diff = int(b1._numberString.size() - b2._numberString.size());
    for (int i = 0; i < diff; ++i) {
        b2._numberString.insert(0, '0');
    }
    for (int i = int(b1._numberString.size() - 1); i >= 0; --i) {
        int sum = (b1._numberString[i].digitValue()) + (b2._numberString[i].digitValue()) + carry;
        carry = 0;
        if (sum < BASE || i == 0) {
            results.insert(0, QString::number(sum));
        }
        else {
            results.insert(0, QString::number(sum % BASE));
            carry = 1;
        }
    }
    return BigInteger(results);
}

BigInteger BigInteger::addll(const long long &other) {
    return this->add(BigInteger(other));
}

BigInteger BigInteger::addstr(const QString &other) {
    return this->add(BigInteger(other));
}


BigInteger BigInteger::subtract(const BigInteger &other) {
    BigInteger b1 = *this;
    BigInteger b2 = other;
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            return b1.negate().add(b2.negate()).negate();
        }
        if (b1.isNegative() && !b2.isNegative()) {
            return b1.negate().add(b2).negate();
        }
        return b2.negate().add(b1);
    }
    QString results;
    int n = 0;
    int p = 0;
    bool takeOffOne = false;
    bool shouldBeTen = false;

    if (b1 < b2) {
        //Negative answer
        QString t = b2.subtract(*this).negate().getString();
        for (int i = 1; i < t.length(); ++i) {
            if (t[i] != '0') break;
            t.remove(1, 1);
        }
        return BigInteger(t);
    }

    //This next if-block fixes the case where the digit difference is greater than 1
    //100 - 5 is an example. This code adds 0's to make it, for example, 100 - 05, which
    //allows the rest of the subtraction code to work.
    if (b1._numberString.size() - b2.getString().size() > 1) {
        for (int i = 0; i < b1._numberString.size() - b2.getString().size() - 1; ++i) {
            b2._numberString.insert(0, '0');
        }
    }
    int i = int(b1._numberString.size() - 1);
    for (int j = int(b2._numberString.size() - 1); j >= 0; --j) {
        if (((b1._numberString[i].digitValue()) < (b2._numberString[j].digitValue())) && i > 0) {
            n = char((b1._numberString[i].digitValue()) + BASE);
            takeOffOne = true;
            if (j > 0 || b1._numberString[i - 1] != '0') {
                p = char((b1._numberString[i - 1].digitValue()) - 1);
                if (p == -1) {
                    p += BASE;
                    shouldBeTen = true;
                }
                takeOffOne = false;
            }
            if (shouldBeTen) {
                int index = i - 1;
                for (int a = i - 1; (b1._numberString[a].digitValue()) == 0; --a) {
                    b1._numberString[a] = static_cast<char>(p + '0');
                    --index;
                }
                int t = (b1._numberString[index].digitValue()) - 1;
                b1._numberString[index] = static_cast<char>(t + '0');
            }
            b1._numberString[i - 1] = static_cast<char>(p + '0');
            shouldBeTen = false;
        }
        QString ss;
        if (((b1._numberString[i].digitValue()) == (b2._numberString[j].digitValue()))) {
            ss += "0";
        }
        else {
            if (n <= 0) {
                ss += QString::number((b1._numberString[i].digitValue()) - (b2._numberString[j].digitValue()));
            } else {
                ss += QString::number(n - (b2._numberString[j].digitValue()));
            }
        }

        results.insert(0, ss);
        --i;
        n = 0;
    }
    if (takeOffOne) {
        QString number;
        for (int j = b1._numberString.length() - b2._numberString.length() - 1; j >= 0; --j) {
            if (b1._numberString[j] == '0') {
                number += "0";
                continue;
            }
            else {
                number.insert(0, b1._numberString[j]);
                int t = number.toInt();
                --t;
                b1._numberString.replace(0, number.size(), QString::number(t));
                break;
            }
        }
    }
    while (i >= 0) {
        QString ss;
        if (i == 0) {
            if (b1._numberString[i].digitValue() != 0) {
                ss += QString::number(b1._numberString[i].digitValue());
                results.insert(0, ss);
            }
        }
        else {
            ss += QString::number(b1._numberString[i].digitValue());
            results.insert(0, ss);
        }

        --i;
    }
    //In the case of all 0's, we only want to return one of them
    if (results.count('0') == results.size()) {
        results = "0";
    }
    else if (results[0] == '0') {
        int index = 1;
        while (results[index] == '0') index++;
        results = results.mid(index);
    }
    return BigInteger(results);
}

BigInteger BigInteger::subtractll(const long long &other) {
    return this->subtract(BigInteger(other));
}

BigInteger BigInteger::subtractstr(const QString &other) {
    return this->subtract(BigInteger(other));
}

BigInteger BigInteger::multiply(BigInteger other) {
    BigInteger b1 = other > *this ? other : *this;
    BigInteger b2 = other > *this ? *this : other;
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            return b1.negate().multiply(b2.negate());
        }
        else if (b1.isNegative() && !b2.isNegative()) {
            return b1.negate().multiply(b2).negate();
        }
        else {
            return b2.negate().multiply(b1).negate();
        }
    }
    if (b1 == 0 || b2 == 0) return 0;
    int carry = 0;
    int zeroCounter = 0;
    BigInteger b = 0;
    
    for (int i = 0; i < b1._numberString.size() - b2._numberString.size(); ++i) {
        b2._numberString.insert(0, '0');
    }
    for (int i = (b2._numberString.size() - 1); i >= 0; --i) {
        QString rr;
        for (int j = int(b1._numberString.size() - 1); j >= 0; --j) {
            int val = ((b2._numberString[i].digitValue()) * (b1._numberString[j].digitValue())) + carry;
            carry = 0;
            if (val > 9 && j != 0) {
                carry = val / 10;
                rr.insert(0, QString::number(val % 10));
            }
            else {
                rr.insert(0, QString::number(val));
            }
        }
        if (zeroCounter > 0) {
            for (int x = 0; x < zeroCounter; ++x) {
                rr.append("0");
            }
        }
        ++zeroCounter;
        b += BigInteger(rr);
    }
    if (b._numberString.count('0') != b._numberString.size()) {
        int index = 0;
        while (b._numberString[index] == '0') index++;
        b._numberString.remove(0, index);
    } else {
        //In the case of all 0's, we only want to return one of them
        b.setString("0");
    }
    return b;
}

BigInteger BigInteger::multiplyll(const long long &other) {
    if (other == 0)
        return 0;
    if (other == 1)
        return *this;
    auto original = *this;
    for (auto i = 0; i < other - 1; ++i) {
        *this += original;
    }
    return *this;
}

BigInteger BigInteger::multiplystr(const QString &other) {
    return this->multiply(BigInteger(other));
}

BigInteger BigInteger::divide(BigInteger other) {
    if (other == 0) {
        qDebug() << "You cannot divide by 0!";
    }
    BigInteger b1 = *this, b2 = other;
    bool sign = false;
    if (b1.isNegative() && b2.isNegative()) {
        b1.negate();
        b2.negate();
    }
    else if (b1.isNegative() && !b2.isNegative()) {
        b1.negate();
        sign = true;
    }
    else if (!b1.isNegative() && b2.isNegative()) {
        b2.negate();
        sign = true;
    }
    BigInteger quotient = 0;
    while (b1 >= b2) {
        b1 -= b2;
        ++quotient;
    }
    if (sign) quotient.negate();
    return quotient;
}

BigInteger BigInteger::dividell(const long long &other) {
    return this->divide(BigInteger(other));
}

BigInteger BigInteger::dividestr(const QString &other) {
    return this->divide(BigInteger(other));
}

BigInteger BigInteger::pow(int exponent) {
    if (exponent < 0) qDebug() << "Powers less than 0 are not supported" ;
    if (exponent == 0) return BigInteger("1");
    if (exponent == 1) return *this;
    BigInteger result = 1, base = *this;
    while (exponent) {
        if (exponent & 1) {
            result *= base;
        }
        exponent >>= 1;
        base *= base;
    }
    return result;
}

QString BigInteger::getString() {
    return this->_numberString;
}

BigInteger BigInteger::setString(const QString &newStr) {
    this->_numberString = newStr;
    return *this;
}

BigInteger BigInteger::negate() {
    if (this->_numberString[0] == '-') {
        this->_numberString.remove(0, 1);
    }
    else {
        this->_numberString.insert(0, '-');
    }
    return *this;
}

BigInteger BigInteger::trimLeadingZeros() {
    BigInteger b = *this;
    int index = 0;
    while (b._numberString[index] == '0') index++;
    if (index != b._numberString.size())
        b._numberString.remove(0, index);
    return b;
}

bool BigInteger::equals(const BigInteger &other) {
    return this->_numberString == other._numberString;
}

bool BigInteger::equals(const long long &other) {
    return this->getString() == QString::number(other);
}

bool BigInteger::equals(const QString &other) {
    return this->getString() == other;
}

size_t BigInteger::digits() {
    return this->_numberString.length() - static_cast<int>(this->isNegative());
}

bool BigInteger::isNegative() const {
    return this->_numberString[0] == '-';
}

bool BigInteger::isPositive() {
    return !this->isNegative();
}

bool BigInteger::isEven() {
    return this->_numberString.back().digitValue() % 2 == 0;
}

bool BigInteger::isOdd() {
    return !this->isEven();
}

BigInteger BigInteger::abs() const {
    BigInteger result = *this;
    if (isNegative()) return result.negate();
    return result;
}

BigInteger operator+(BigInteger b1, const BigInteger &b2) {
    return b1.add(b2);
}

BigInteger operator+(BigInteger b1, const long long &b2) {
    return b1.addll(b2);
}

BigInteger operator+(BigInteger b1, const QString &b2) {
    return b1.addstr(b2);
}

BigInteger operator-(BigInteger b1, const BigInteger &b2) {
    return b1.subtract(b2);
}

BigInteger operator-(BigInteger b1, const long long &b2) {
    return b1.subtractll(b2);
}

BigInteger operator-(BigInteger b1, const QString &b2) {
    return b1.subtractstr(b2);
}

BigInteger operator*(BigInteger b1, const BigInteger &b2) {
    return b1.multiply(b2);
}

BigInteger operator*(BigInteger b1, const long long &b2) {
    return b1.multiplyll(b2);
}

BigInteger operator*(BigInteger b1, const QString &b2) {
    return b1.multiplystr(b2);
}

BigInteger operator/(BigInteger b1, const BigInteger &b2) {
    return b1.divide(b2);
}

BigInteger operator/(BigInteger b1, const long long &b2) {
    return b1.dividell(b2);
}

BigInteger operator/(BigInteger b1, const QString &b2) {
    return b1.dividestr(b2);
}

BigInteger operator^(BigInteger b1, const int &b2) {
    return b1.pow(b2);
}

bool operator==(BigInteger b1, const BigInteger &b2) {
    return b1.equals(b2);
}

bool operator==(BigInteger b1, const long long &b2) {
    return b1.equals(b2);
}

bool operator==(BigInteger b1, const QString &b2) {
    return b1.equals(b2);
}

bool operator>(BigInteger b1, const BigInteger &b2) {
    if (b1.isNegative() || b2.isNegative()) {
        if (b1.isNegative() && b2.isNegative()) {
            BigInteger bt = b2;
            b1._numberString.remove(0, 1);
            bt._numberString.remove(0, 1);
            return b1 < bt;
        }
        else {
            return !(b1.isNegative() && !b2.isNegative());
        }
    }
    b1 = b1.trimLeadingZeros();
    auto c = BigInteger(b2);
    c = c.trimLeadingZeros();
    if (b1 == c) {
        return false;
    }
    if (b1._numberString.size() > c._numberString.size()) {
        return true;
    }
    else if (c._numberString.size() > b1._numberString.size()) {
        return false;
    }
    else {
        for (int i = 0; i < b1._numberString.size(); ++i) {
            if (b1[i] == static_cast<unsigned int>(c._numberString[i].digitValue())) {
                continue;
            }
            return b1[i] > static_cast<unsigned int>(c._numberString[i].digitValue());
        }
    }
    return false;
}

bool operator<(BigInteger b1, const BigInteger &b2) {
    return !(b1 == b2) && !(b1 > b2);
}

bool operator>=(BigInteger b1, const BigInteger &b2) {
    return b1 > b2 || b1 == b2;
}

bool operator<=(BigInteger b1, const BigInteger &b2) {
    return b1 < b2 || b1 == b2;
}

unsigned int BigInteger::operator[](int index) {
    if (this->_numberString[index] == '-') {
        qDebug() << "You cannot get the negative sign from the number" ;
    }
    return static_cast<unsigned int>(this->_numberString[index].digitValue());
}

BigInteger& BigInteger::operator=(const BigInteger &other) {
    this->_numberString = other._numberString;
    return *this;
}

BigInteger& BigInteger::operator=(const long long &other) {
    this->_numberString = QString::number(other);
    return *this;
}

BigInteger& BigInteger::operator=(const QString &other) {
    this->_numberString = other;
    return *this;
}

BigInteger& BigInteger::operator+=(const BigInteger &other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator+=(const long long &other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator+=(const QString &other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger &other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator-=(const long long &other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator-=(const QString &other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger &other) {
    *this = *this * other;
    return *this;
}

BigInteger& BigInteger::operator*=(const long long &other) {
    *this = *this * other;
    return *this;
}

BigInteger& BigInteger::operator*=(const QString &other) {
    *this = *this * other;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger &other) {
    *this = *this / other;
    return *this;
}

BigInteger& BigInteger::operator/=(const long long &other) {
    *this = *this / other;
    return *this;
}

BigInteger& BigInteger::operator/=(const QString &other) {
    *this = *this / other;
    return *this;
}

BigInteger& BigInteger::operator++() {
    *this += BigInteger("1");
    return *this;
}

BigInteger& BigInteger::operator--() {
    *this -= BigInteger("1");
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger t(this->getString());
    ++(*this);
    return t;
}

BigInteger BigInteger::operator--(int) {
    BigInteger t(this->getString());
    --(*this);
    return t;
}

QDataStream& operator>>(QDataStream &in, BigInteger &x) {
    in >> x._numberString;
    return in;
}

QDataStream& operator<<(QDataStream &out, const BigInteger &x) {
    out << x._numberString;
    return out;
}
