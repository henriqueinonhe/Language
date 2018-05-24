#include "formatter.h"

Formatter::Formatter()
{

}

QString Formatter::format(QString string)
{
    std::for_each(processors.begin(), processors.end(), [](const ProcessorEntry processor)
    {
        processor.processString(string);
    });

    return string;
}
