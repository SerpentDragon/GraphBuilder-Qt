#ifndef GRAPH_H
#define GRAPH_H

#include <QPointF>
#include <vector>
#include <string>
#include <qcolor.h>
#include "settings.h"
#include "parsetree.h"

using GraphLines = std::vector<std::pair<QColor, std::vector<QPointF>>>;

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

    void setChecked(int);

    void setUnchecked(int);

    GraphLines calculateFunctions(double, double, double) const;

private:

    std::vector<FunctionGraphic> functions_;
};

#endif // GRAPH_H
