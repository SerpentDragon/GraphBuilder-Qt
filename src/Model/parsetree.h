#ifndef PARSETREE_H
#define PARSETREE_H

#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <numbers>
#include <unordered_set>
#include "../Controller/settings.h"

double findRoots(double, void*);

enum PRIORITY : unsigned short { SUMSUB = 1, MULDIV, POW, MID = 5, HIGHEST = 10};

enum class ANGLE : unsigned short { RADIANS = 0, DEGREES };

class ParseTree
{
private:

    struct Node
    {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node>  right = nullptr;
        std::string data;
    };

    using NodePtr = std::shared_ptr<Node>;

    friend double findRoots(double, void*);

public:

    void setExpression(const std::string&);

    double evalTree(double, ANGLE) const;

    double evalTree(NodePtr, double, ANGLE) const;

    std::vector<NodePtr> findLimitFunctions() const;

private:

    NodePtr makeTree(const std::string&);

    std::string checkBrackets(const std::string&);

    int lastOp(const std::string&);

    PRIORITY priority(char);

    std::vector<NodePtr> findLimitFunctions(NodePtr) const;

private:

    NodePtr root_;
};

#endif // PARSETREE_H
