#include "parsetree.h"

namespace Functions
{
std::vector<std::string> functions = { "sin", "cos", "tg", "ctg", "asin", "acos", "atg",
                                      "actg", "sinh", "cosh", "tgh", "ctgh", "ln", "lg", "log", "abs", "sqrt" };

bool isFunction(std::string function)
{
    if (function[0] == '-') function = function.substr(1);

    for (const auto& func : functions)
    {
        if (function.find(func) == 0) return true;
    }

    return false;
}
}

void ParseTree::setExpression(const std::string& func)
{
    std::string expression = checkBrackets(func);
    root_ = makeTree(expression);
}

double ParseTree::evalTree(double x)
{
    return evalTree(root_, x);
}

ParseTree::~ParseTree()
{
    delete root_;
}

ParseTree::Node* ParseTree::makeTree(const std::string& expr)
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
                node->left = makeTree(expr.substr(1));
                node->right = nullptr;
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

                    node->left = makeTree(arg);
                    node->right = nullptr;
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

    return node;
}

double ParseTree::evalTree(Node* node, double x)
{
    if (node->left == nullptr)
    {
        std::string expr = node->data;
        int mul = expr[0] == '-' ? -1 : 1;

        if (expr == "e") return mul * M_E;
        else if (expr == "π") return mul * M_PI;
        else if (expr == "x") return mul * x;
        else return std::stod(expr);
    }
    else
    {
        double left = evalTree(node->left, x);
        double right = node->right == nullptr ? 0 : evalTree(node->right, x);
        double res;

        if (node->data == "+") return left + right;
        else if (node->data == "-") return node->right == nullptr ? -left : left - right;
        else if (node->data == "*") return left * right;
        else if (node->data == "/") return left / right;
        else if (node->data == "%") return fmod(left, right);
        else if (node->data == "^") return powf(left, right);
        else if (node->data == "sin") return sin(left);
        else if (node->data == "cos") return cos(left);
        else if (node->data == "tg") return tan(left);
        else if (node->data == "ctg") return 1 / tan(left);
        else if (node->data == "asin") return asin(left);
        else if (node->data == "acos") return acos(left);
        else if (node->data == "atg") return atan(left);
        else if (node->data == "actg") return atan(1 / left);
        else if (node->data == "sinh") return sinh(left);
        else if (node->data == "cosh") return cosh(left);
        else if (node->data == "tgh") return tanh(left);
        else if (node->data == "ctgh") return 1 / tanh(left);
        else if (node->data == "ln") return log(left);
        else if (node->data == "lg") return log10(left);
        else if (node->data == "log") return log(right) / log(left);
        else if (node->data == "abs") return fabs(left);
        else if (node->data == "sqrt") return sqrt(left);
        else return 0;
    }
}

std::string ParseTree::checkBrackets(const std::string expr)
{
    while (expr[0] == '(')
    {
        int bracketsCounter = 0;
        bool stop = false;

        for (int i = 0; i < expr.size(); i++)
        {
            if (expr[i] == '(') bracketsCounter++;
            else if (expr[i] == ')') bracketsCounter--;

            if (bracketsCounter == 0)
            {
                if (i == expr.size() - 1)
                {
                    expr = expr.substr(1, expr.size() - 2);
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

    return expr;
}

int ParseTree::lastOp(const std::string& expr)
{
    int minPriority = 5;
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

int ParseTree::priority(char op)
{
    switch (op)
    {
    case '+': case '-': return 1;
    case '*': case '/': case '%': return 2;
    case '^': return 3;
    default: return 10;
    }
}
