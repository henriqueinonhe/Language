#include "basicprocessortokenrecord.h"

BasicProcessorTokenRecord::BasicProcessorTokenRecord()
{

}

BasicProcessorTokenRecord::BasicProcessorTokenRecord(const CoreToken &token, const unsigned int operatorPosition, const BasicProcessorTokenRecord::Associativity associativity) :
    associativity(associativity)
{
    /* Validation here basically consists of:
     * 1 - Token should be an operator, therefore it should take at least one argument!
     * 2 - Operator position should be consistent with the number of arguments the operator takes! */

    if(!token.getType().isOperator())
    {
        throw std::invalid_argument(QString("The token (" + token.getString() + ") associated with the record should be an operator!").toStdString());
    }

    const unsigned int numberOfArguments = token.getType().getNumberOfArguments();
    if(operatorPosition > numberOfArguments)
    {
        throw std::invalid_argument(QString("The operator position (" + QString::number(operatorPosition) + ") is inconsistent with the number of arguments(" + QString::number(numberOfArguments) + ") the operator takes!").toStdString());
    }

    tokenSubRecordList.push_back(TokenSubRecord(token.getString(), operatorPosition, token.getType().getNumberOfArguments()));
}

bool BasicProcessorTokenRecord::hasTokenAsSubRecord(const QString &token) const
{
    return std::any_of(tokenSubRecordList.begin(), tokenSubRecordList.end(), [&token](const TokenSubRecord &subRecord)
    {
        return subRecord.token == token;
    });
}

BasicProcessorTokenRecord::TokenSubRecord BasicProcessorTokenRecord::findTokenSubRecord(const QString &token) const
{
    for(auto subRecordIter = tokenSubRecordList.begin(); subRecordIter != tokenSubRecordList.end(); subRecordIter++)
    {
        if(subRecordIter->token == token)
        {
            return *subRecordIter;
        }
    }

    throw std::invalid_argument("There is no sub record associated with that token!");
}

bool BasicProcessorTokenRecord::checkExistsTokenSubRecordDuplicate(const QString &token) const
{
    return std::any_of(tokenSubRecordList.begin(), tokenSubRecordList.end(), [&token](const TokenSubRecord &record)
    {
        return record.token == token;
    });
}
