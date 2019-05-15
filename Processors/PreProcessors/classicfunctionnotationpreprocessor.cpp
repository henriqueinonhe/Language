#include "classicfunctionnotationpreprocessor.h"
#include <QString>

ClassicFunctionNotationPreProcessor::ClassicFunctionNotationPreProcessor(Signature *signature) :
    signature(signature)
{

}

QString ClassicFunctionNotationPreProcessor::processString(const QString &string) const
{
    return QString(""); //Placeholder
}

QString ClassicFunctionNotationPreProcessor::toString() const
{
    return "Classic Function Notation Pre Processor";
}
