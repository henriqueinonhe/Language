#ifndef CLASSICFUNCTIONNOTATIONPREPROCESSOR_H
#define CLASSICFUNCTIONNOTATIONPREPROCESSOR_H

#include "stringprocessor.h"
#include "signature.h"

class ClassicFunctionNotationPreProcessor : virtual StringProcessor
{
public:
    ClassicFunctionNotationPreProcessor(Signature *signature);

    QString processString(const QString &string) const;
    QString toString() const;

private:
    Signature *signature;
};

#endif // CLASSICFUNCTIONNOTATIONPREPROCESSOR_H
