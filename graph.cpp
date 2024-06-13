#include "graph.h"

std::shared_ptr<Graph> Graph::graph_ = nullptr;

std::shared_ptr<Graph> Graph::getGraph()
{
    if (graph_ == nullptr) graph_ = std::make_shared<Graph>();

    return graph_;
}

void Graph::addFunction(const std::string& func, ANGLE angle)
{
    FunctionGraphic fg;
    fg.parseTree_.setExpression(func);

    fg.angle = angle;

    fg.isDrawable_ = true;
    fg.color_ = QColor(rand() % 101 + 100, rand() % 101 + 100, rand() % 101 + 100);

    functions_.emplace_back(fg);
}

void Graph::removeFunction()
{

}

void Graph::displayFunction(int index)
{
    if (0 <= index && index <= functions_.size())
    {
        functions_[index].isDrawable_ = true;
    }
}

void Graph::hideFunction(int index)
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
        QPointF point;

        for(double value = left; value <= right; value += step)
        {
            point.setX(value);
            point.setY(functions_[i].parseTree_.evalTree(value, functions_[i].angle));

            functionValues.emplace_back(point);
        }

        calculations.insert({ i, { color, functionValues } });
    }

    return calculations;
}

std::vector<double> Graph::findRupturePoints(int index, double left, double right) const
{
    auto limitFunctions = functions_[index].parseTree_.findLimitFunctions();

    const gsl_root_fsolver_type *T = gsl_root_fsolver_brent;
    gsl_root_fsolver *s = gsl_root_fsolver_alloc(T);

    std::vector<double> breakPoints;

    for(const auto& node : limitFunctions)
    {
        auto param = std::make_tuple(node, functions_[index].parseTree_, functions_[index].angle);

        gsl_function F;
        F.function = &findRoots;
        F.params = reinterpret_cast<void*>(&param);

        double root;
        gsl_root_fsolver_set(s, &F, left, right);

        try
        {
            int status, iter = 0;
            do
            {
                status = gsl_root_fsolver_iterate(s);
                root = gsl_root_fsolver_root(s);
                left = gsl_root_fsolver_x_lower(s);
                right = gsl_root_fsolver_x_upper(s);
                status = gsl_root_test_interval(left, right, 0, std::pow(10, MathParams::Precision));
            } while (status == GSL_CONTINUE && ++iter < 100);
        }
        catch(const std::exception& ex)
        {
            continue;
        }

        breakPoints.emplace_back(root);
    }

    gsl_root_fsolver_free(s);

    return breakPoints;
}

std::vector<QPointF> Graph::calculateBrokenInterval(int index, double left, double right) const
{
    auto breakPoints = findRupturePoints(index, left, right);
    auto func = functions_[index];

    std::vector<QPointF> values(1, { left, func.parseTree_.evalTree(left, func.angle) });

    if (!breakPoints.empty())
    {
        for(const double x : breakPoints)
        {
            double leftX = x - MathParams::Epsilon;
            QPointF point{ leftX, func.parseTree_.evalTree(leftX, func.angle)};
            values.emplace_back(point);

            double rightX = x + MathParams::Epsilon;
            point = { rightX, func.parseTree_.evalTree(rightX, func.angle)};
            values.emplace_back(point);
        }
    }

    values.emplace_back(QPointF{ right, func.parseTree_.evalTree(right, func.angle) });

    return values;
}
