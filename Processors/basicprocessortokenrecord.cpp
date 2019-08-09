#include "basicprocessortokenrecord.h"
#include <QDataStream>

BasicProcessorTokenRecord::Associativity BasicProcessorTokenRecord::associativityFromString(const QString &string)
{
    if(string == "left")
    {
        return Associativity::Left;
    }
    else if(string == "right")
    {
    return Associativity::Right;
    }
    else
    {
        QString errorMsg;
        errorMsg += "\"";
        errorMsg += string;
        errorMsg += "\" cannot be converted into an associativity!";
        throw invalid_argument(errorMsg.toStdString());
    }
}

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

QDataStream &operator <<(QDataStream &stream, const BasicProcessorTokenRecord &record)
{
    stream << record.tokenSubRecordList;
    stream << record.associativity;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, BasicProcessorTokenRecord &record)
{
    stream >> record.tokenSubRecordList;
    stream >> record.associativity;
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const BasicProcessorTokenRecord::Associativity &associativity)
{
    stream << static_cast<unsigned int>(associativity);
    return stream;
}

QDataStream &operator >>(QDataStream &stream, BasicProcessorTokenRecord::Associativity &associativity)
{
    unsigned int val;
    stream >> val;
    associativity = static_cast<BasicProcessorTokenRecord::Associativity>(val);
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const BasicProcessorTokenRecord::TokenSubRecord &subRecord)
{
    stream << subRecord.token << subRecord.operatorPosition << subRecord.numberOfArguments;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, BasicProcessorTokenRecord::TokenSubRecord &subRecord)
{
    stream >> subRecord.token >> subRecord.operatorPosition >> subRecord.numberOfArguments;
    return stream;
}
