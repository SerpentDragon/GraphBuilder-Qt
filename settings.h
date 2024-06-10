#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

namespace WidgetParams
{
    constexpr int plotterSegmentSize = 60;
    constexpr int listViewItemHeight = 30;
};

namespace AllowedSymbols
{
    const QString constants("1234567890)eπx");
    const QString digitAllowedSymbols("+-*/%(^.,1234567890");
    const QString operators("+-*/%^(,");
};

namespace MathParams
{
    constexpr double Precision = 4.0;
}

#endif // SETTINGS_H
