#ifndef PARSETREE_H
#define PARSETREE_H

#include <cmath>
#include <string>
#include <vector>

class ParseTree
{
public:

    void SetExpression(std::string&);

    double EvalTree(double = 0);

    ~ParseTree();

private:

    struct Node
    {
        Node* left = nullptr;
        Node* right = nullptr;
        std::string data;

        ~Node()
        {
            delete left;
            delete right;
        }
    };

private:

    Node* MakeTree(const std::string&);

    double EvalTree(Node*, double);

    std::string CheckBrackets(std::string);

    int LastOp(const std::string&);

    int Priority(char);

private:

    Node* root_;
};


#endif // PARSETREE_H
