#ifndef PARSETREE_H
#define PARSETREE_H

#include <cmath>
#include <string>
#include <memory>
#include <vector>
#include <numbers>
#include "settings.h"
#include <unordered_set>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

// double findRoots(double, void*);

enum PRIORITY : unsigned short { SUMSUB = 1, MULDIV, POW, MID = 5, HIGHEST = 10};

enum class ANGLE : unsigned short { RADIANS = 0, DEGREES };

class ParseTree
{
public:

    void setExpression(const std::string&);

    double evalTree(double, ANGLE) const;

    std::vector<double> findBreakPoints(double, double) const;

private:

    struct Node
    {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node>  right = nullptr;
        std::string data;
    };

private:

    std::shared_ptr<Node> makeTree(const std::string&);

    std::vector<std::shared_ptr<ParseTree::Node>> findLimitFunctions(std::shared_ptr<Node>) const;

    double evalTree(std::shared_ptr<Node>, double, ANGLE angle) const;

    std::string checkBrackets(const std::string&);

    int lastOp(const std::string&);

    PRIORITY priority(char);

    friend double findRoots(double, void*);

private:

    std::shared_ptr<Node> root_;
};

#endif // PARSETREE_H
