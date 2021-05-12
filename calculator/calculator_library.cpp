#include "calculator_library.hpp"


Expression::Expression(Token token) : token(token){}
Expression::Expression(Token token, Expression arg) : token(token), args{arg} {}
Expression::Expression(Token token, Expression arg_left, Expression arg_right) : token(token), args{arg_left, arg_right} {}

my_math::Сalculator::Сalculator (const std::string& input) : input(input) {}
my_math::Сalculator::Сalculator(std::string&& input) : input(std::move(input)) {}

Token my_math::Сalculator::parse_token()
{
    std::regex reg_operand(R"(^(0|[1-9]\d*)([.,]\d+)?)");
    std::regex reg_operation(R"(^(\^|\/|\(|\[|\)|\]|sin|cos|abs|mod|sqrt|\+|-|\*))");
    std::regex reg_mr(R"(^(MR))");

    std::smatch res_oprand;
    std::smatch res_operation;
    std::smatch res_mr;

        if (input.empty())
        {
            return "";
        }
        else if (std::regex_search(input, res_oprand, reg_operand)) 
        {
            auto temp_token = res_oprand[0].str();
            input.erase(0, temp_token.size());

            return std::stod(temp_token);
        }
        else if (std::regex_search(input, res_operation, reg_operation))
        {
            auto temp_token = res_operation[0].str();
            input.erase(0, temp_token.size());

            return temp_token;
        }
        else if(std::regex_search(input, res_mr, reg_mr))
        {
            auto temp_token = res_mr[0].str();
            input.erase(0, temp_token.size());
            
            if (input[0] != '[' && input[0] != '(' && !isdigit(input[0]))
            {
                if (!memory.empty())
                    return memory.back();
                else
                    throw std::invalid_argument("The memory is empty");
            }

            return temp_token;
        }

        throw std::invalid_argument("Invalid token");
}

Expression my_math::Сalculator::parse_unary_expression()
{
    auto temp_token = parse_token();

    if (temp_token.index() == 0)
    {
        return Expression(temp_token);
    }
    else if (get<1>(temp_token) == "(" || get<1>(temp_token) == "[")
    {
        auto expression = parse_binary_expression(0);

        auto parse_bracket = parse_token();

        if(get<1>(temp_token) == "(" && get<1>(parse_bracket) != ")")
            throw std::invalid_argument(R"(Expect: ')' )");
        
        if(get<1>(temp_token) == "[" && get<1>(parse_bracket) != "]")
            throw std::invalid_argument(R"(Expect: ']' )");

        return expression;
    }

    auto arg_oper = parse_unary_expression();
    
    return Expression(temp_token, arg_oper);
}

size_t my_math::Сalculator::get_prioryty(const Token& token)
{
    if (token.index() == 1)
    {
        auto tokenid = get<1>(token);

        if (tokenid == "-") return 1;
        if (tokenid == "+") return 1;
        if (tokenid == "*") return 2;
        if (tokenid == "/") return 2;
        if (tokenid == "sin") return 2;
        if (tokenid == "cos") return 2;
        if (tokenid == "sqrt") return 2;
        if (tokenid == "mod") return 2;
        if (tokenid == "abs") return 2;
        if (tokenid == "MR") return 2;
        if (tokenid == "^") return 3;
    }

    return 0;
}

Expression my_math::Сalculator::parse_binary_expression(size_t min_prioryty)
{
    auto expression_left = parse_unary_expression();

    while(true)
    {
        auto temp_operation = parse_token();
        auto priority = get_prioryty(temp_operation);

        if(priority <= min_prioryty)
        {
            input.insert(0, get<1>(temp_operation));
            return expression_left;
        }

        auto expression_right = parse_binary_expression(priority);
        expression_left = Expression(temp_operation, expression_left, expression_right);
    }
}

void my_math::Сalculator::finding_errors()
{
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

    std::replace(input.begin(), input.end(), ',', '.');
    
    if (input.empty())
        throw std::invalid_argument("Empty entrance");

    auto left_brackets_round = std::count(input.begin(), input.end(), '(');
    auto right_brackets_round = std::count(input.begin(), input.end(), ')');

    auto left_brackets_square = std::count(input.begin(), input.end(), '[');
    auto right_brackets_square = std::count(input.begin(), input.end(), ']');

    if (left_brackets_round != right_brackets_round)
        throw std::logic_error(R"(Different number of brackets '(' and ')')");

    if (left_brackets_square != right_brackets_square)
        throw std::logic_error(R"(Different number of brackets '[' and ']')");
}

double my_math::Сalculator::make_calculation(Expression& exp)
{
    switch (exp.args.size())
    {
    case 2: 
    {
        auto left_oper = make_calculation(exp.args[0]);
        auto right_oper = make_calculation(exp.args[1]);

        if (get<1>(exp.token) == "-") return left_oper - right_oper;
        if (get<1>(exp.token) == "+") return left_oper + right_oper;
        if (get<1>(exp.token) == "*") return left_oper * right_oper;
        if (get<1>(exp.token) == "/") return left_oper / right_oper;
        if (get<1>(exp.token) == "^") return std::pow(left_oper, right_oper);
        if (get<1>(exp.token) == "mod") return static_cast<int>(left_oper) % static_cast<int>(right_oper);
        
    }
    case 1:
    {
        auto operand = make_calculation(exp.args[0]);

        if (get<1>(exp.token) == "-") return -operand;
        if (get<1>(exp.token) == "+") return +operand;
        if (get<1>(exp.token) == "sin") return std::sin(operand);
        if (get<1>(exp.token) == "cos") return std::cos(operand);
        if (get<1>(exp.token) == "sqrt") return std::sqrt(operand);
        if (get<1>(exp.token) == "abs") return std::abs(operand);
        if (get<1>(exp.token) == "MR") return get_memory(operand);
    }
    case 0:
        return get<0>(exp.token);
    }
}

double my_math::Сalculator::get_memory(size_t index)
{
    if (index >= memory.size())
        throw std::out_of_range("Index of MR is out of range");

    return memory.at(index);
}

bool my_math::Сalculator::check_for_MW()
{
    std::regex reg_mw(R"(^(MW))");
    std::smatch res_mw;

    if (std::regex_search(input, res_mw, reg_mw))
    {
        if (temporary_buffer) 
        {
            auto temp_token = res_mw[0].str();
            input.erase(0, temp_token.size());

            if (input[0] != '[' && input[0] != '(' && !isdigit(input[0]))
            {
                memory.push_back(temporary_buffer.value());                
            }
            else
            {
                auto parse_index = parse_unary_expression();
                auto index = make_calculation(parse_index);
                
                if (index >= memory.size())
                    memory.resize(index + 1);

                memory.at(index) = temporary_buffer.value();
            }
                
            return true;
        }
        else
        {
            throw std::invalid_argument("No data to write to memory");
        }
    }

    return false;
}

double my_math::Сalculator::calculate()
{
    try 
    {
        finding_errors();

        if (check_for_MW())
            return 0;

        auto parsed_expression = parse_binary_expression(0);

        temporary_buffer = make_calculation(parsed_expression);

        return temporary_buffer.value();
    } 
    catch(const std::exception& ex)
    {
        std::cout << ex.what() << "\n";
    }

    return 0;
}

double my_math::Сalculator::calculate(const std::string& input)
{
    this->input = input;

    return calculate();
}

double my_math::Сalculator::calculate(std::string&& input)
{
    this->input = std::move(input);

    return calculate();
}

size_t my_math::Сalculator::memory_size() const { return memory.size(); }

void my_math::Сalculator::memory_clear() { memory.clear(); }