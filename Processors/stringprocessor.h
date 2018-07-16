﻿#ifndef STRINGPROCESSOR_H
#define STRINGPROCESSOR_H

#include <QString>

class StringProcessor
{
public:
    virtual QString processString(const QString &string) const = 0;

    virtual QString toString() const = 0;
};

#endif // STRINGPROCESSOR_H