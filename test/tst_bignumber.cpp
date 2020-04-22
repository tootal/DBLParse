#include <QtTest>

#include <bignumber.h>

// add necessary includes here

class BigNumberTest : public QObject
{
    Q_OBJECT
    
public:
    BigNumberTest();
    ~BigNumberTest();
    
private slots:
    void test_Addition();
    void test_Subtraction();
    void test_Multiplication();
    void test_Division();
    void test_Exponentiation();
    void test_Compare();
    void test_Other();
};

BigNumberTest::BigNumberTest()
{
    
}

BigNumberTest::~BigNumberTest()
{
    
}

void BigNumberTest::test_Addition()
{
    //Addition
    QVERIFY((BigNumber(50) + BigNumber(32)).getString() == "82");
    QVERIFY((BigNumber(5) + BigNumber(622)).getString() == "627");
    QVERIFY((BigNumber("-33") + BigNumber("8")).getString() == "-25");
    QVERIFY((BigNumber("15535") + BigNumber("0")).getString() == "15535");
    QVERIFY((BigNumber("126") + BigNumber("39285")).getString() == "39411");
    QVERIFY((BigNumber("0") + BigNumber("0")).getString() == "0");
    QVERIFY(BigNumber(5) + 10 == 15);
    QVERIFY(BigNumber("-41") + 40 == -1);
    BigNumber ad1(600);
    ad1 += 50;
    ad1 += "50";
    QVERIFY(ad1.getString() == "700");
    QVERIFY(ad1 == 700);
}

void BigNumberTest::test_Subtraction()
{
    //Subtraction
    QVERIFY((BigNumber("50") - BigNumber("32")).getString() == "18");
    QVERIFY((BigNumber("50") - BigNumber("60")).getString() == "-10");
    QVERIFY((BigNumber("0") - BigNumber("46")).getString() == "-46");
    QVERIFY((BigNumber("50") - BigNumber("50")).getString() == "0");
    QVERIFY((BigNumber("482847") - BigNumber("89787941")).getString() == "-89305094");
    QVERIFY((BigNumber("6828") - BigNumber("1")).getString() == "6827");
    QVERIFY((BigNumber("100") - BigNumber("50")).getString() == "50");
    QVERIFY((BigNumber("42") - BigNumber("49")).getString() == "-7");
    QVERIFY((BigNumber("100") - BigNumber("5")) == 95);
    BigNumber sb1 = 200;
    sb1 -= 40;
    QVERIFY(sb1 == 160);
    sb1 = sb1 - 180;
    QVERIFY(sb1 == "-20");
    sb1 -= "20";
    QVERIFY(sb1 == BigNumber(-40));
}

void BigNumberTest::test_Multiplication()
{
    //Multiplication
    QVERIFY((BigNumber("4") * BigNumber("12")).getString() == "48");
    QVERIFY((BigNumber("3002") * BigNumber("1")).getString() == "3002");
    QVERIFY((BigNumber("99") * BigNumber("0")).getString() == "0");
    QVERIFY((BigNumber("-5") * BigNumber("5")).getString() == "-25");
    QVERIFY((BigNumber("-33") * BigNumber("-2")).getString() == "66");
    QVERIFY((BigNumber("283") * BigNumber("382871")).getString() == "108352493");
    BigNumber ml1 = 4;
    ml1 *= 6;
    QVERIFY(ml1 == "24");
    ml1 = BigNumber(5) * 6;
    QVERIFY(ml1 == 30);
    ml1 *= "5000";
    QVERIFY(ml1 == 150000);
}

void BigNumberTest::test_Division()
{
    //Division
    QVERIFY(BigNumber("25").divide(BigNumber("5")) == 5);
    QVERIFY(BigNumber("48").divide(BigNumber("6")) == 8);
    QVERIFY(BigNumber("100").divide(BigNumber("5")) == 20);
    BigNumber dv1 = 100;
    dv1 /= 25;
    QVERIFY(dv1 == 4);
    dv1 = dv1 / dv1;
    QVERIFY(dv1 == 1);
    dv1 /= 1;
    QVERIFY(dv1 == 1);
    dv1 = -5;
    dv1 /= 5;
    QVERIFY(dv1 == -1);
    dv1 = 3000;
    dv1 /= 300;
    QVERIFY(dv1 == 10);
    dv1 = 25485;
    dv1 /= 5;
    QVERIFY(dv1 == "5097");
}

void BigNumberTest::test_Exponentiation()
{
    //Exponentiation
    QVERIFY((BigNumber("2").pow(3)).getString() == "8");
    QVERIFY((BigNumber("1").pow(38)).getString() == "1");
    QVERIFY((BigNumber("5").pow(2)).getString() == "25");
    QVERIFY((BigNumber("10").pow(10)).getString() == "10000000000");
    QVERIFY((BigNumber("5").pow(1)).getString() == "5");
    QVERIFY((BigNumber("5").pow(0)).getString() == "1");
    QVERIFY((BigNumber("-5").pow(2)).getString() == "25");
}

void BigNumberTest::test_Compare()
{
    //Equals
    QVERIFY(BigNumber("4") == BigNumber("4"));
    QVERIFY(BigNumber("-3") == BigNumber("-3"));
    QVERIFY(BigNumber("0") == BigNumber("0"));
    QVERIFY(BigNumber("938283828178273") == BigNumber("938283828178273"));

    //Greater than
    QVERIFY(BigNumber("5") > BigNumber("2"));
    QVERIFY(BigNumber("30") > BigNumber("-40"));
    QVERIFY(BigNumber("-5") > BigNumber("-10"));
    QVERIFY(BigNumber("0") > BigNumber("-1"));

    //Less than
    QVERIFY(BigNumber("10") < BigNumber("20"));
    QVERIFY(BigNumber("-5") < BigNumber("0"));
    QVERIFY(BigNumber("30") < BigNumber("30000"));

    //Greater than or equal to
    QVERIFY(BigNumber("5") >= BigNumber("0"));
    QVERIFY(BigNumber("-5") >= BigNumber("-5"));
    QVERIFY(BigNumber("-5") >= BigNumber("-10"));
    QVERIFY(BigNumber("0") >= BigNumber("0"));
    QVERIFY(BigNumber("32") >= BigNumber("-32"));
    QVERIFY(BigNumber("2") >= BigNumber("0001"));

    //Less than or equal to
    QVERIFY(BigNumber("5") <= BigNumber("10"));
    QVERIFY(BigNumber("0") <= BigNumber("0"));
    QVERIFY(BigNumber("-5") <= BigNumber("0"));
    QVERIFY(BigNumber("30") <= BigNumber("30"));
    QVERIFY(BigNumber("400") <= BigNumber("392342"));
}

void BigNumberTest::test_Other()
{
    //Index
    QVERIFY(BigNumber("423")[1] == 2);
    QVERIFY(BigNumber("0")[0] == 0);
    QVERIFY(BigNumber("-5")[1] == 5);

    //Even
    QVERIFY(BigNumber("426").isEven());
    QVERIFY(BigNumber("-20").isEven());

    //Odd
    QVERIFY(BigNumber("83").isOdd());
    QVERIFY(BigNumber("-27").isOdd());

    //Positive
    QVERIFY(BigNumber("38").isPositive());

    //Negative
    QVERIFY(BigNumber("-28382").isNegative());

    //Increment/Decrement operators
    QVERIFY(BigNumber("5")--.getString() == "5");
    QVERIFY((--BigNumber("5")).getString() == "4");
    QVERIFY(BigNumber("10")++.getString() == "10");
    QVERIFY((++BigNumber("10")).getString() == "11");

    BigNumber a("10");
    a++;
    QVERIFY(a.getString() == "11");
    ++a;
    QVERIFY(a.getString() == "12");
    a--;
    QVERIFY(a.getString() == "11");
    --a;
    QVERIFY(a.getString() == "10");

    //Absolute value
    QVERIFY(BigNumber("45").abs().getString() == "45");
    QVERIFY(BigNumber("-325").abs().getString() == "325");

    //Digits
    QVERIFY(BigNumber("28374765").digits() == 8);
    QVERIFY(BigNumber("-3092").digits() == 4);

    //Set string
    QVERIFY(BigNumber("234").setString("-45").getString() == "-45");

    //Assignment operator
    BigNumber c(10);
    c = 5;
    QVERIFY(c == 5);
    QVERIFY(c == BigNumber(5));
    QVERIFY(c == BigNumber("5"));
    c = "83833";
    QVERIFY(c == 83833);
    QVERIFY(c == BigNumber(83833));
    QVERIFY(c == BigNumber("83833"));

    //Equals testing
    BigNumber d(40);
    QVERIFY(d == 40);
    QVERIFY(d == "40");
    QVERIFY(d == BigNumber("40"));
    QVERIFY(d == BigNumber(40));
    d = 40;
    QVERIFY(d == 40);
    d = "40";
    QVERIFY(d == 40);
}

QTEST_APPLESS_MAIN(BigNumberTest)

#include "tst_bignumber.moc"
