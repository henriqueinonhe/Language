#include "basicprocessor.h"

BasicProcessor::BasicProcessor(Signature * const signature) :
    signature(signature)
{

}

void BasicProcessor::addTokenRecord(const QString &token, const unsigned int position, const BasicProcessorTokenRecord::Associativity associativity, const int precedenceRank)
{
    const CoreToken &coreToken = dynamic_cast<CoreToken &>(*(signature->getTokenPointer(token)));
    const BasicProcessorTokenRecord newRecord(coreToken, position, associativity);
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

unsigned int BasicProcessor::getPrecedenceRank(const QString &tokenString) const
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

const BasicProcessorTokenRecord *BasicProcessor::getTokenRecordPtr(const QString &token) const
{
    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++)
    {
        if(iter->token == token)
        {
            return &(*iter);
        }
    }

    throw std::invalid_argument("There is no record associated with this token!");
}

QString BasicProcessor::tokenStringWrapperToString(const BasicProcessor::TokenStringWrapper &tokenString) const
{
    QString string;

    std::for_each(tokenString.begin(), tokenString.end(), [&string](const TokenWrapper &token)
    {
        string += token.token;
        string += " ";
    });

    return string;
}
