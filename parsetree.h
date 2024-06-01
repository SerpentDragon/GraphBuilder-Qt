#ifndef PARSETREE_H
#define PARSETREE_H

#include <cmath>
#include <string>
#include <vector>

class ParseTree
{
public:

    void setExpression(const std::string&);

    double evalTree(double = 0);

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

    Node* makeTree(const std::string&);

    double evalTree(Node*, double);

    std::string checkBrackets(const std::string&);

    int lastOp(const std::string&);

    int priority(char);

private:

    Node* root_;
};


#endif // PARSETREE_H
