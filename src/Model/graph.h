#ifndef GRAPH_H
#define GRAPH_H

#include <tuple>
#include <vector>
#include <string>
#include <memory>
#include <QColor>
#include <QPointF>
#include "parsetree.h"
#include <unordered_map>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>
#include "../Controller/settings.h"

using GraphLines = std::unordered_map<int, std::pair<QColor, std::vector<QPointF>>>;

class Graph
{
private:

    Graph(const Graph&) = delete;

    Graph& operator=(const Graph&) = delete;
    
    std::vector<double> findRupturePoints(int, double, double) const;

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

    void removeFunction(int);

    void displayFunction(int);

    void hideFunction(int);

    GraphLines calculateFunctions(double, double, double) const;

    std::vector<QPointF> calculateBrokenInterval(int, double, double) const;

private:

    static std::shared_ptr<Graph> graph_;

    std::vector<FunctionGraphic> functions_;
};

#endif // GRAPH_H
