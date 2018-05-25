#include "basicprocessor.h"

BasicProcessor::BasicProcessor(Signature * const signature) :
    lexer(signature)
{

}

void BasicProcessor::addTokenRecord(const CoreToken &token, const unsigned int position, const BasicProcessorTokenRecord::Associativity associativity, const int precedenceRank)
{
    const BasicProcessorTokenRecord newRecord(token, position, associativity);
    bool conflictingRecord = std::any_of(tokenRecords.begin(), tokenRecords.end(), [&newRecord](const BasicProcessorTokenRecord &record)
    {
        return newRecord.token == record.token;
    });

    if(conflictingRecord)
    {
        throw std::invalid_argument("There is already a record with this token!");
    }

    if(precedenceRank >= 0)
    {
        auto insertPosition = tokenRecords.begin();
        insertPosition += precedenceRank;
        tokenRecords.insert(insertPosition, newRecord);
    }
    else
    {
        tokenRecords.push_back(newRecord);
    }
}

void BasicProcessor::removeTokenRecord(const QString &tokenString)
{
    bool tokenFound = false;

    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++)
    {
        if(iter->token == tokenString)
        {
            tokenRecords.erase(iter);
            tokenFound = true;
            break;
        }
    }

    if(!tokenFound)
    {
        throw std::invalid_argument("The token prompted to be removed is not in the list!");
    }

}

unsigned int BasicProcessor::getPrecendeRank(const QString &tokenString) const
{
    unsigned int precedenceRank = 0;
    bool tokenFound = false;

    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++, precedenceRank++)
    {
        if(iter->token == tokenString)
        {
            tokenFound = true;
            break;
        }
    }

    if(!tokenFound)
    {
        throw std::invalid_argument("There is no record associated with this token!");
    }

    return precedenceRank;
}
