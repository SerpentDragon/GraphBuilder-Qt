#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

namespace WidgetParams
{
    namespace Plotter
    {
        constexpr int SegmentSize = 60;
    };

    namespace ListViewItem
    {
        constexpr int Height = 45;
        constexpr int CheckBoxSize = Height / 4;
        constexpr int ButtonSize = Height / 4;
        constexpr int MarginValue = 2;
    }
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
    const double Epsilon = std::pow(10, -Precision);
}

#endif // SETTINGS_H
