#ifndef CLASSICFUNCTIONNOTATIONPREPROCESSOR_H
#define CLASSICFUNCTIONNOTATIONPREPROCESSOR_H

#include "stringprocessor.h"

class Signature;

class ClassicFunctionNotationPreProcessor : public StringProcessor
{
public:
    ClassicFunctionNotationPreProcessor(Signature *signature);

    QString processString(const QString &string) const;
    QString toString() const;

    ~ClassicFunctionNotationPreProcessor();

};

#endif // CLASSICFUNCTIONNOTATIONPREPROCESSOR_H
