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

std::vector<std::pair<QColor, std::vector<QPointF>>> Graph::calculateFunctions(const double left, const double right) const
{
    constexpr static int intervals = 1000;
    const double step = (right - left) / intervals;

    std::vector<std::pair<QColor, std::vector<QPointF>>> calculations(functions_.size());

    for(int i = 0; i < functions_.size(); i++)
    {
        QColor color = functions_[i].color_;
        std::vector<QPointF> functionValues;
        QPointF point;

        for(double value = left; value <= right; value += step)
        {
            point.setX(value);

            if (auto result = functions_[i].parseTree_.evalTree(value); !qIsNaN(result))
            {
                point.setY(result);
                functionValues.emplace_back(point);
            }
        }

        calculations[i] = { color, functionValues };
    }

    return calculations;
}
