#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <memory>
#include <QColor>
#include <QPointF>
#include "settings.h"
#include "parsetree.h"

using GraphLines = std::vector<std::pair<QColor, std::vector<QPointF>>>;

class Graph
{
private:

    Graph(const Graph&) = delete;

    Graph& operator=(const Graph&) = delete;

private:

    struct FunctionGraphic
    {
        ParseTree parseTree_;
        ANGLE angle;
        bool isDrawable_;
        QColor color_;
    };

public:

    Graph() = default;

    static std::shared_ptr<Graph> getGraph();

    void addFunction(const std::string&, ANGLE angle);

    void removeFunction();

    void displayFunction(int);

    void hideFunction(int);

    GraphLines calculateFunctions(double, double, double) const;

private:

    static std::shared_ptr<Graph> graph_;

    std::vector<FunctionGraphic> functions_;
};

#endif // GRAPH_H
