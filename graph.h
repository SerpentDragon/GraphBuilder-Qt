#ifndef GRAPH_H
#define GRAPH_H

#include <QPointF>
#include <vector>
#include <string>
#include <qcolor.h>
#include "parsetree.h"

class Graph
{
private:

    struct FunctionGraphic
    {
        ParseTree parseTree_;
        bool isDrawable_;
        QColor color_;
    };

public:

    void addFunction(const std::string&);

    void removeFunction();

    std::vector<std::pair<QColor, std::vector<QPointF>>> calculateFunctions(const double, const double) const;

private:

    std::vector<FunctionGraphic> functions_;
};

#endif // GRAPH_H
