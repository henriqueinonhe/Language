#ifndef STRINGPROCESSOR_H
#define STRINGPROCESSOR_H

#include <QString>

class StringProcessor
{
public:
    StringProcessor();

    virtual QString processString(QString string) = 0;
};

#endif // STRINGPROCESSOR_H
