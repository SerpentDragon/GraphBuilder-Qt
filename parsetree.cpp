#include "parsetree.h"

// function namespaces

namespace Functions
{
    const std::unordered_set<std::string> functions = { "sin", "cos", "tg", "ctg", "asin", "acos", "atg",
                                          "actg", "sinh", "cosh", "tgh", "ctgh", "ln", "lg",
                                          "log", "abs", "sqrt" };

    const std::vector<std::string> trigonometricFunctions = { "sin", "cos", "tg", "ctg", "asin", "acos", "atg",
                                                                    "actg", "sinh", "cosh", "tgh", "ctgh" };

    bool isFunction(std::string function)
    {
        if (function[0] == '-')
        {
            function = function.substr(1);
        }

        function = function.substr(0, function.find('('));

        return functions.contains(function);
    }
};

namespace LimitFunctions
{
    // All the functions in the app which may cause a function break
    const std::unordered_set<std::string> limitFunctions = { "ctgh", "actg", "tg", "ctg", "/", "%" };
};

// friend function to calculate node values

// double findRoots(double x, void* param)
// {
//     auto data = reinterpret_cast<std::pair<ParseTree*, std::shared_ptr<ParseTree::Node>>*>(param);

//     ParseTree* tree = data->first;
//     auto node = data->second;

//     return tree->evalTree(node, x);
// }

// ParseTree class

void ParseTree::setExpression(const std::string& func)
{
    std::string expression = checkBrackets(func);
    root_ = makeTree(expression);
}

double ParseTree::evalTree(double x, ANGLE angle) const
{
    return evalTree(root_, x, angle);
}

std::vector<double> ParseTree::findBreakPoints(double left, double right) const
{
    // auto limitFunctions = findLimitFunctions(root_);

    // const gsl_root_fsolver_type *T = gsl_root_fsolver_brent;
    // gsl_root_fsolver *s = gsl_root_fsolver_alloc(T);

    std::vector<double> breakPoints;

    // for(const auto& node : limitFunctions)
    // {
    //     auto params = std::pair(this, node);

    //     gsl_function F;
    //     F.function = &findRoots;
    //     F.params = reinterpret_cast<void*>(&params);

    //     double root;
    //     gsl_root_fsolver_set(s, &F, left, right);

    //     int status;
    //     do
    //     {
    //         status = gsl_root_fsolver_iterate(s);
    //         root = gsl_root_fsolver_root(s);
    //         left = gsl_root_fsolver_x_lower(s);
    //         right = gsl_root_fsolver_x_upper(s);
    //         status = gsl_root_test_interval(left, right, 0, std::pow(10, MathParams::Precision));
    //     } while (status == GSL_CONTINUE);

    //     breakPoints.emplace_back(root);
    // }

    // gsl_root_fsolver_free(s);

    return breakPoints;
}

std::shared_ptr<ParseTree::Node> ParseTree::makeTree(const std::string& expr)
{
    Node* node = new Node();

    int index = lastOp(expr);

    if (index == -1 || index == 0)
    {
        if (Functions::isFunction(expr))
        {
            if (index == 0)
            {
                node->data = "-";
                node->left = nullptr;
                node->right = makeTree(expr.substr(1));
            }
            else
            {
                node->data = expr.substr(0, expr.find('('));

                if (node->data == "log")
                {
                    std::string arg1 = checkBrackets(expr.substr(expr.find('(') + 1, expr.find(',') - expr.find('(')));
                    std::string arg2 = checkBrackets(expr.substr(expr.find(',') + 1, expr.size() - 2 - expr.find(',')));

                    node->left = makeTree(arg1);
                    node->right = makeTree(arg2);
                }
                else
                {
                    std::string arg = checkBrackets(expr.substr(expr.find('(') + 1, expr.size() - 2 - expr.find('(')));

                    node->left = nullptr;
                    node->right = makeTree(arg);
                }
            }
        }
        else
        {
            node->data = expr;
            node->left = nullptr;
            node->right = nullptr;
        }
    }
    else
    {
        std::string leftTree = checkBrackets(expr.substr(0, index));
        std::string rightTree = checkBrackets(expr.substr(index + 1, expr.size() - 1 - index));

        node->data = expr[index];
        node->left = makeTree(leftTree);
        node->right = makeTree(rightTree);
    }

    return std::make_shared<Node>(*node);
}

std::vector<std::shared_ptr<ParseTree::Node>> ParseTree::findLimitFunctions(std::shared_ptr<Node> node) const
{
    if (node == nullptr) return {};

    std::vector<std::shared_ptr<ParseTree::Node>> limitFunctions;

    if (LimitFunctions::limitFunctions.contains(node->data))
    {
        limitFunctions.emplace_back(node->right);
    }

    auto left = findLimitFunctions(node->left);
    auto right = findLimitFunctions(node->right);

    limitFunctions.insert(limitFunctions.end(), left.begin(), left.end());
    limitFunctions.insert(limitFunctions.end(), right.begin(), right.end());

    return limitFunctions;
}

double ParseTree::evalTree(std::shared_ptr<Node> node, double x, ANGLE angle) const
{
    std::string expr = node->data;

    if (node->right == nullptr)
    {
        int mul = expr[0] == '-' ? -1 : 1;

        if (expr.contains('e')) return mul * std::numbers::e;
        else if (expr.contains("π")) return mul * std::numbers::pi;
        else if (expr.contains('x')) return mul * x;
        else return std::stod(expr);
    }
    else
    {
        double left = node->left == nullptr ? 0 : evalTree(node->left, x, angle);
        double right = evalTree(node->right, x, angle);

        if (angle == ANGLE::DEGREES)
        {
            if (std::any_of(Functions::trigonometricFunctions.begin(),
                            Functions::trigonometricFunctions.end(), [&expr](const std::string& func) { return expr.contains(func); }))
            {
                right = right * 180 / std::numbers::pi;
            }
        }

        if (expr == "+") return left + right;
        else if (expr == "-") return node->right == nullptr ? -left : left - right;
        else if (expr == "*") return left * right;
        else if (expr == "/") return left / right;
        else if (expr == "%") return fmod(left, right);
        else if (expr == "^") return powf(left, right);
        else if (expr == "sin") return sin(right);
        else if (expr == "cos") return cos(right);
        else if (expr == "tg") return tan(right);
        else if (expr == "ctg") return 1 / tan(right);
        else if (expr == "asin") return asin(right);
        else if (expr == "acos") return acos(right);
        else if (expr == "atg") return atan(right);
        else if (expr == "actg") return atan(1 / right);
        else if (expr == "sinh") return sinh(right);
        else if (expr == "cosh") return cosh(right);
        else if (expr == "tgh") return tanh(right);
        else if (expr == "ctgh") return 1 / tanh(right);
        else if (expr == "ln") return log(right);
        else if (expr == "lg") return log10(right);
        else if (expr == "log") return log(left) / log(right);
        else if (expr == "abs") return fabs(right);
        else if (expr == "sqrt") return sqrt(right);
        else return 0;
    }
}

std::string ParseTree::checkBrackets(const std::string& expr)
{
    std::string expression = expr;

    while (expression[0] == '(')
    {
        int bracketsCounter = 0;
        bool stop = false;

        for (int i = 0; i < expression.size(); i++)
        {
            if (expression[i] == '(') bracketsCounter++;
            else if (expression[i] == ')') bracketsCounter--;

            if (bracketsCounter == 0)
            {
                if (i == expression.size() - 1)
                {
                    expression = expression.substr(1, expr.size() - 2);
                }
                else
                {
                    stop = true;
                    break;
                }
            }
        }

        if (stop) break;
    }

    return expression;
}

int ParseTree::lastOp(const std::string& expr)
{
    unsigned short minPriority = PRIORITY::MID;
    int lastOpIndex = -1;
    int bracketsCounter = 0;

    for (int i = 0; i < expr.size(); i++)
    {
        if (expr[i] == '(') bracketsCounter++;
        else if (expr[i] == ')') bracketsCounter--;

        int prior = priority(expr[i]);
        if (prior <= minPriority && bracketsCounter == 0)
        {
            minPriority = prior;
            lastOpIndex = i;
        }
    }

    return lastOpIndex;
}

PRIORITY ParseTree::priority(char op)
{
    switch (op)
    {
    case '+': case '-': return PRIORITY::SUMSUB;
    case '*': case '/': case '%': return PRIORITY::MULDIV;
    case '^': return PRIORITY::POW;
    default: return PRIORITY::HIGHEST;
    }
}

