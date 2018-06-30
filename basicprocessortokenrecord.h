#ifndef BASICPROCESSORTOKENRECORD_H
#define BASICPROCESSORTOKENRECORD_H

#include "coretoken.h"

class BasicProcessor;
class BasicPreProcessor;
class BasicPostProcessor;

class BasicProcessorTokenRecord
{
public:
    enum class Associativity
    {
        Left,
        Right
    };

    BasicProcessorTokenRecord();
    BasicProcessorTokenRecord(const CoreToken &token,
                              const unsigned int operatorPosition,
                              const Associativity associativity);
    struct TokenSubRecord
    {
        TokenSubRecord()
        {
        }

        TokenSubRecord(const QString &token,
                       const unsigned int operatorPosition,
                       const unsigned int numberOfArguments) :
            token(token),
            operatorPosition(operatorPosition),
            numberOfArguments(numberOfArguments)
        {}

        QString token;
        unsigned int operatorPosition;
        unsigned int numberOfArguments;
    };

    bool hasTokenAsSubRecord(const QString &token) const;
    TokenSubRecord findTokenSubRecord(const QString &token) const;

private:

    bool checkExistsTokenSubRecordDuplicate(const QString &token) const;

    QVector<TokenSubRecord> tokenSubRecordList;
    Associativity associativity;

    friend class BasicProcessor;
    friend class BasicPreProcessor;
    friend class BasicPostProcessor;
};

#endif // BASICPROCESSORTOKENRECORD_H
