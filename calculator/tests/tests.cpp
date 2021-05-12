#include <gtest/gtest.h>
#include "calculator_library.hpp"


TEST(calculator_test, test_functionality1)
{
    my_math::Сalculator cal("2 + 6 + sin(15) - cos(12) + 5^2 *(12.5 - 3.47)");

    double res = 2 + 6 + std::sin(15) - std::cos(12) + std::pow(5,2) *(12.5 - 3.47);

    ASSERT_EQ(res, cal.calculate());
}

TEST(calculator_test, test_functionality2)
{
    my_math::Сalculator cal;

    auto res_my = cal.calculate("abs(-8+3) + 5*sin(24) - 4^2 + 8 - 25*cos(0)/12 + 2/3");

    auto res = std::abs(-8+3) + 5*std::sin(24) - std::pow(4,2) + 8 - 25*std::cos(0)/12 + 2.0/3.0;

    ASSERT_EQ(res, res_my);
}

TEST(calculator_test, test_functionality3_fractional)
{
    my_math::Сalculator cal;

    auto res_my = cal.calculate("12.277 * 15.678 * (sqrt(84.265) - 12.84*cos(15.98)/sin(abs(-24.37))) + 12.87^0.98");

    auto res = 12.277 * 15.678 * (std::sqrt(84.265) - 12.84*std::cos(15.98)/std::sin(std::abs(-24.37))) + std::pow(12.87,0.98);

    ASSERT_EQ(res, res_my);
}

TEST(calculator_test, test_functionality4)
{
    std::string expression = "12^2 * (-1) + 254/sqrt(14 + 12 - 2^2)*sin(cos(abs(-15)))^(-1) - 254";
    my_math::Сalculator cal(expression);

    auto res = std::pow(12, 2) * (-1) + 254.0/std::sqrt(14 + 12 - std::pow(2,2)) * std::pow(std::sin(std::cos(std::abs(-15))), -1) - 254;

    cal.calculate();
    cal.calculate("MW[12]");

    ASSERT_EQ(13, cal.memory_size());

    ASSERT_EQ(0, cal.calculate("MR[2+2+2+1+3]"));

    ASSERT_EQ(res, cal.calculate("MR[6*6*2/6]"));
}

TEST(calculator_test, test_for_MW_and_MR)
{
    my_math::Сalculator cal("2 + 5 * 9^2 + cos(12) - 25 + 12");

    auto res_cal = cal.calculate();
    auto res = 2 + 5 * std::pow(9 , 2) + std::cos(12) - 25 + 12;

    ASSERT_EQ(res, res_cal);

    cal.calculate("MW");

    ASSERT_EQ(res, cal.calculate("MR"));
    ASSERT_EQ(1, cal.memory_size());

    cal.calculate("15 + 12 - 3 + 5");
    cal.calculate("MW[1]");

    auto res1 = 15 + 12 - 3 + 5;

    ASSERT_EQ(res1, cal.calculate("MR[0+0+0+0*210 + 1]"));
    ASSERT_EQ(2, cal.memory_size());

    ASSERT_EQ(res, cal.calculate("MR[15*0]"));
}

TEST(calculator_test, exception_test_for_MW_and_MR)
{
    my_math::Сalculator cal;
    
    //No data to write to memory
    ASSERT_EQ(0, cal.calculate("MW"));
}

TEST(calculator_test, exception_test_for_empty_in)
{
    my_math::Сalculator cal;

    //Empty entrance
    ASSERT_EQ(0, cal.calculate());
}

TEST(calculator_test, exception_test_for_different_number_of_brackets)
{
    my_math::Сalculator cal("5+(12*8-3+14");

    //Different number of brackets '(' and ')'
    ASSERT_EQ(0, cal.calculate());

    //Different number of brackets '[' and ']'
    ASSERT_EQ(0, cal.calculate("MW[4"));
}

TEST(calculator_test, exception_test_for_MR_exceptions)
{
    my_math::Сalculator cal;

    //The memory is empty
    ASSERT_EQ(0, cal.calculate("MR"));

    //Index of MR is out of rang
    ASSERT_EQ(0, cal.calculate("MR[12^2]"));
}

TEST(calculator_test, exception_test_unknown_token_exceptions)
{
    my_math::Сalculator cal;

    //Invalid token
    ASSERT_EQ(0, cal.calculate("yes"));
}



int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}