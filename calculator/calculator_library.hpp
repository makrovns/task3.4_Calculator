#pragma once
#include <iostream>
#include <regex>
#include <string>
#include <algorithm>
#include <variant>
#include <vector>
#include <optional>

namespace
{
    using operand = double;
    using operation = std::string;
    using Token = std::variant<operand, operation>;
    using std::get;

    class Expression
    {
    public:
        Expression(Token token);
        Expression(Token token, Expression arg);
        Expression(Token token, Expression arg_left, Expression arg_right);

        Token token;
        std::vector<Expression> args;
    };
}

namespace my_math
{
    class Сalculator
    {
    public:

        Сalculator() = default;
        Сalculator(const std::string& input);
        Сalculator(std::string&& input);
        
        double calculate();
        double calculate(const std::string& input);
        double calculate(std::string&& input);
        
        size_t memory_size() const;
        void memory_clear();

    private:

        void finding_errors();
        size_t get_prioryty(const Token& token);
        Token parse_token();
        Expression parse_unary_expression();
        Expression parse_binary_expression(size_t prioryty);
        bool check_for_MW();
        double get_memory(size_t index);
        double make_calculation(Expression& exp);

        std::string input;
        std::optional<double> temporary_buffer;
        std::vector<double> memory;
    };
}