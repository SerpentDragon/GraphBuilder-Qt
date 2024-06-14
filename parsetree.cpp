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
    // All the functions in the app which may cause a function rupture
    const std::unordered_set<std::string> limitFunctions = { "ctgh", "actg", "tg", "ctg", "/", "%" };
};

// function to calculate roots

double findRoots(double x, void* param)
{
    auto data = reinterpret_cast<std::tuple<ParseTree::NodePtr, ParseTree, ANGLE>*>(param);

    auto node = std::get<0>(*data);
    ANGLE angle = std::get<2>(*data);

    return std::get<1>(*data).evalTree(node, x, angle);
}

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

std::vector<ParseTree::NodePtr> ParseTree::findLimitFunctions() const
{
    return findLimitFunctions(root_);
}

ParseTree::NodePtr ParseTree::makeTree(const std::string& expr)
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

std::vector<ParseTree::NodePtr> ParseTree::findLimitFunctions(NodePtr node) const
{
    if (node == nullptr) return {};

    std::vector<ParseTree::NodePtr> limitFunctions;

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
