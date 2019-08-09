#include "tablesignature.h"
#include "punctuationtoken.h"
#include <QDataStream>
#include "variabletoken.h"
#include "bindingtoken.h"

TableSignature::TableSignature(QDataStream &stream)
{
    deserialize(stream);
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

const Token *TableSignature::getTokenPointer(const QString &token) const
{
    auto targetTokenIter = tokenTable.find(token.toStdString());
    if(targetTokenIter == tokenTable.end())
    {
        QString errorMsg = "This token (\"";
        errorMsg += token;
        errorMsg += "\") is absent from this signature! Which means that this token was not declared!";

        throw std::invalid_argument(errorMsg.toStdString());
    }
    return targetTokenIter->second.get();
}

void TableSignature::pushTokenPointerToTable(const Token &token)
{
    tokenTable.emplace(token.getString().toStdString(), unique_ptr<Token>(token.getAllocatedClone()));
}

QDataStream &operator <<(QDataStream &stream, const TableSignature &signature)
{
    signature.serialize(stream);
    return stream;
}

QDataStream &operator >>(QDataStream &stream, TableSignature &signature)
{
    signature.deserialize(stream);
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
    stream << tokenTable.size();
    for(const auto &element : tokenTable)
    {
        stream << QString(element.first.data());
        stream << *element.second;
    }
}

void TableSignature::deserialize(QDataStream &stream)
{
    unsigned int size;
    stream >> size;
    for(unsigned int count = 0; count < size; count++)
    {
        QString key;
        stream >> key;

        tokenTable.emplace(key.toStdString(), deserializeTokenPtr(stream));
    }
}

unique_ptr<Token> TableSignature::deserializeTokenPtr(QDataStream &stream) const
{
    QString tokenClass;
    stream >> tokenClass;

    if(tokenClass == "PunctuationToken")
    {
        return unique_ptr<Token>(new PunctuationToken(stream));
    }
    else if(tokenClass == "CoreToken")
    {
        return unique_ptr<Token>(new CoreToken(stream));
    }
    else if(tokenClass == "BindingToken")
    {
        return unique_ptr<Token>(new BindingToken(stream));
    }
    else if(tokenClass == "VariableToken")
    {
        return unique_ptr<Token>(new VariableToken(stream));
    }
    else
    {
        QString errorMsg;
        errorMsg += "\"";
        errorMsg += tokenClass;
        errorMsg += "\" is not a valid token class!";
        throw invalid_argument(errorMsg.toStdString());
    }
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
    token.reset(Token::deserializePtr(stream));
    return stream;
}

