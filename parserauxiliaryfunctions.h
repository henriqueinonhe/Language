#ifndef PARSERAUXILIARYFUNCTIONS_H
#define PARSERAUXILIARYFUNCTIONS_H

#include <QVector>
#include <QString>
#include "typetoken.h"

class ParserAuxiliaryFunctions
{
public:
    static unsigned int findMainOperatorIndexWithDelimiters(const QVector<TypeToken> &sentence,
                                                            const TypeToken &leftDelimiter,
                                                            const TypeToken &rightDelimiter);
};

#endif // PARSERAUXILIARYFUNCTIONS_H
