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

std::vector<std::pair<QColor, std::vector<QPointF>>> Graph::calculateFunctions(const double left, const double right) const
{
    static int intervals = std::pow(10, MathParams::Precision);
    const double step = (right - left) / intervals;

    std::vector<std::pair<QColor, std::vector<QPointF>>> calculations;

    for(int i = 0; i < functions_.size(); i++)
    {
        if (functions_[i].isDrawable_ == false) continue;

        QColor color = functions_[i].color_;
        std::vector<QPointF> functionValues;
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
