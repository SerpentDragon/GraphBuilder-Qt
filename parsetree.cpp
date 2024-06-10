#include "parsetree.h"

namespace Functions
{
    const std::vector<std::string> functions = { "sin", "cos", "tg", "ctg", "asin", "acos", "atg",
                                          "actg", "sinh", "cosh", "tgh", "ctgh", "ln", "lg",
                                          "log", "abs", "sqrt" };

    bool isFunction(const std::string& function)
    {
        std::string func = function;

        if (func[0] == '-') func = func.substr(1);

        for (const auto& f : functions)
        {
            if (func.find(f) == 0) return true;
        }

        return false;
    }
}

namespace LimitFunctions
{
    const std::vector<std::string> limitFunctions = { "ctgh", "actg", "tg", "ctg", "/", "%" };
};

enum PRIORITY : unsigned short { SUMSUB = 1, MULDIV, POW, MID = 5, HIGHEST = 10};

void ParseTree::setExpression(const std::string& func)
{
    std::string expression = checkBrackets(func);
    root_ = makeTree(expression);
}

double ParseTree::evalTree(double x) const
{
    return evalTree(root_, x);
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

double ParseTree::evalTree(std::shared_ptr<Node> node, double x) const
{
    if (node->right == nullptr)
    {
        std::string expr = node->data;
        int mul = expr[0] == '-' ? -1 : 1;

        if (expr.contains('e')) return mul * std::numbers::e;
        else if (expr.contains("π")) return mul * std::numbers::pi;
        else if (expr.contains('x')) return mul * x;
        else return std::stod(expr);
    }
    else
    {
        double left = node->left == nullptr ? 0 : evalTree(node->left, x);
        double right = evalTree(node->right, x);

        if (node->data == "+") return left + right;
        else if (node->data == "-") return node->right == nullptr ? -left : left - right;
        else if (node->data == "*") return left * right;
        else if (node->data == "/") return left / right;
        else if (node->data == "%") return fmod(left, right);
        else if (node->data == "^") return powf(left, right);
        else if (node->data == "sin") return sin(right);
        else if (node->data == "cos") return cos(right);
        else if (node->data == "tg") return tan(right);
        else if (node->data == "ctg") return 1 / tan(right);
        else if (node->data == "asin") return asin(right);
        else if (node->data == "acos") return acos(right);
        else if (node->data == "atg") return atan(right);
        else if (node->data == "actg") return atan(1 / right);
        else if (node->data == "sinh") return sinh(right);
        else if (node->data == "cosh") return cosh(right);
        else if (node->data == "tgh") return tanh(right);
        else if (node->data == "ctgh") return 1 / tanh(right);
        else if (node->data == "ln") return log(right);
        else if (node->data == "lg") return log10(right);
        else if (node->data == "log") return log(left) / log(right);
        else if (node->data == "abs") return fabs(right);
        else if (node->data == "sqrt") return sqrt(right);
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
