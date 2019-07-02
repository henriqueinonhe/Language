#include "tablesignature.h"
#include "punctuationtoken.h"
#include <QDataStream>

TableSignature::TableSignature(QDataStream &stream)
{
    stream >> tokenTable;
}

TableSignature::TableSignature()
{
    //Initializing
    addToken(PunctuationToken("("));
    addToken(PunctuationToken(")"));
}

bool TableSignature::tokenIsAlreadyPresentInSignature(const Token &token) const
{
    return tokenTable.find(token.getString().toStdString()) != tokenTable.end();
}

const Token *TableSignature::getTokenPointer(const QString &token)
{
    if(tokenTable.find(token.toStdString()) == tokenTable.end())
    {
        QString errorMsg = "This token (\"";
        errorMsg += token;
        errorMsg += "\") is absent from this signature! Which means that this token was not declared!";

        throw std::invalid_argument(errorMsg.toStdString().data());
    }
    return tokenTable[token.toStdString()].get();
}

void TableSignature::pushTokenPointerToTable(const Token &token)
{
    unique_ptr<Token> ptr;
    ptr.reset(token.getAllocatedClone());

    tokenTable.emplace(token.getString().toStdString(), unique_ptr<Token>(token.getAllocatedClone()));
}

QDataStream &operator <<(QDataStream &stream, const TableSignature &signature)
{
    stream << signature.tokenTable;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, TableSignature &signature)
{
    stream >> signature.tokenTable;
    return stream;
}

void TableSignature::addToken(const Token &token)
{
    if(tokenIsAlreadyPresentInSignature(token))
    {
        QString errorMsg;
        errorMsg += "This token (\"";
        errorMsg += token.getString();
        errorMsg += "\") is already present in the table!";

        throw std::invalid_argument(errorMsg.toStdString());
    }
    else
    {
        pushTokenPointerToTable(token);
    }
}

bool TableSignature::operator==(const TableSignature &other) const
{
    return tokenTable == other.tokenTable;
}

bool TableSignature::operator!=(const TableSignature &other) const
{
    return !(*this == other);
}

bool TableSignature::equalTokenTable(const TableSignature &other) const
{
    if(tokenTable.size() != other.tokenTable.size())
    {
        return false;
    }

    auto thisTokenTableIter = tokenTable.begin();
    for(unsigned int index = 0; index < tokenTable.size(); index++)
    {
        if(!other.tokenIsAlreadyPresentInSignature(*thisTokenTableIter->second))
        {
            return false;
        }
        thisTokenTableIter++;
    }

    return true;
}

void TableSignature::serialize(QDataStream &stream) const
{
    stream << tokenTable;
}

void TableSignature::unserialize(QDataStream &stream)
{
    stream >> tokenTable;
}

QDataStream &operator <<(QDataStream &stream, const unique_ptr<Token> &token)
{
    stream << *token;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, unique_ptr<Token> &token)
{
    //NOTE I could take some precautions regarding deserialization on a Token Table that has already been
    //initialized and is in use, so whoever is using the old pointers don't lose references!
    token.reset(Token::unserializePtr(stream));
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const unordered_map<string, unique_ptr<Token> > &map)
{
    stream << map.size();
    for(const auto &element : map)
    {
        stream << QString(element.first.data());
        stream << *element.second;
    }
    return stream;
}

QDataStream &operator >>(QDataStream &stream, unordered_map<string, unique_ptr<Token> > &map)
{
    unsigned int size;
    stream >> size;
    for(unsigned int count = 0; count < size; count++)
    {
        QString key;
        stream >> key;
        unique_ptr<Token> token;
        stream >> token;

        map.emplace(key.toStdString(), unique_ptr<Token>(token.release()));
    }
    return stream;
}
