#ifndef PARSETREE_H
#define PARSETREE_H

#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <numbers>

enum PRIORITY : unsigned short;

class ParseTree
{
public:

    void setExpression(const std::string&);

    double evalTree(double = 0) const;

private:

    struct Node
    {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node>  right = nullptr;
        std::string data;
    };

private:

    std::shared_ptr<Node> makeTree(const std::string&);

    double evalTree(std::shared_ptr<Node>, double) const;

    std::string checkBrackets(const std::string&);

    int lastOp(const std::string&);

    PRIORITY priority(char);

private:

    std::shared_ptr<Node> root_;
};


#endif // PARSETREE_H
