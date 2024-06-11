#include "graph.h"

void Graph::addFunction(const std::string& func)
{
    FunctionGraphic fg;
    fg.parseTree_.setExpression(func);

    fg.isDrawable_ = true;
    fg.color_ = QColor(rand() % 101 + 100, rand() % 101 + 100, rand() % 101 + 100);

    functions_.emplace_back(fg);
}

void Graph::removeFunction()
{

}

void Graph::setChecked(int index)
{
    if (0 <= index && index <= functions_.size())
    {
        functions_[index].isDrawable_ = true;
    }
}

void Graph::setUnchecked(int index)
{
    if (0 <= index && index <= functions_.size())
    {
        functions_[index].isDrawable_ = false;
    }
}

GraphLines Graph::calculateFunctions(double left, double right, double coordinatesFactor) const
{
    double step = right - left;

    step = coordinatesFactor < 1 ?
               step /= std::pow(10, MathParams::Precision) :
               step *= std::pow(10, -MathParams::Precision);

    GraphLines calculations;

    for(int i = 0; i < functions_.size(); i++)
    {
        if (functions_[i].isDrawable_ == false) continue;

        QColor color = functions_[i].color_;
        std::vector<QPointF> functionValues;
        // auto breakPoints = functions_[i].parseTree_.findBreakPoints(left, right);
        QPointF point;

        for(double value = left; value <= right; value += step)
        {
            point.setX(value);
            point.setY(functions_[i].parseTree_.evalTree(value));

            functionValues.emplace_back(point);
        }

        calculations.push_back({ color, functionValues });
    }

    return calculations;
}
