#ifndef FUNCTIONPARSER_H
#define FUNCTIONPARSER_H

#include <vector>
#include <string>
#include <qcolor.h>
#include "parsetree.h"

class FunctionParser
{
private:

    struct FunctionGraphic
    {
        ParseTree pareTree_;
        bool isDrawable_;
        QColor color_;
    };

public:

    void addFunction(const std::string&);

    void removeFunction();

    const std::vector<FunctionGraphic>& getFunctions() const;

private:

    std::vector<FunctionGraphic> functions_;
};

#endif // FUNCTIONPARSER_H
