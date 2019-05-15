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

        TokenSubRecord(const CoreToken &token,
                       const unsigned int operatorPosition,
                       const unsigned int numberOfArguments) :
            token(token.getString()),
            operatorPosition(operatorPosition),
            numberOfArguments(numberOfArguments)
        {
            /* Validation here basically consists of:
             * 1 - Token should be an operator, therefore it should take at least one argument!
             * 2 - Operator position should be consistent with the number of arguments the operator takes! */

            if(!token.getType().isOperator())
            {
                throw std::invalid_argument(QString("The token (" + token.getString() + ") associated with the record should be an operator!").toStdString());
            }

            if(operatorPosition > numberOfArguments)
            {
                throw std::invalid_argument(QString("The operator position (" + QString::number(operatorPosition) + ") is inconsistent with the number of arguments(" + QString::number(numberOfArguments) + ") the operator takes!").toStdString());
            }
        }

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
