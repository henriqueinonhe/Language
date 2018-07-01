#include "basicprocessortokenrecord.h"

BasicProcessorTokenRecord::BasicProcessorTokenRecord()
{

}

BasicProcessorTokenRecord::BasicProcessorTokenRecord(const CoreToken &token, const unsigned int operatorPosition, const BasicProcessorTokenRecord::Associativity associativity) :
    associativity(associativity)
{
    tokenSubRecordList.push_back(TokenSubRecord(token, operatorPosition, token.getType().getNumberOfArguments()));
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
