#include "functionparser.h"

void FunctionParser::addFunction(const std::string& func)
{
    FunctionGraphic fg;
    fg.pareTree_ = ParseTree();
    fg.pareTree_.setExpression(func);

    fg.isDrawable_ = true;
    fg.color_ = QColor(rand() % 201 + 100, rand() % 201 + 100, rand() % 201 + 100);

    functions_.emplace_back(fg);
}

void FunctionParser::removeFunction()
{

}

const std::vector<FunctionParser::FunctionGraphic>& FunctionParser::getFunctions() const
{
    return functions_;
}
