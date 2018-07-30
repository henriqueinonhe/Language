#include "tablesignature.h"

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
    return std::any_of(tokenTable.begin(), tokenTable.end(), [&token](const shared_ptr<Token> &tableToken)
    {
        return token.getString() == tableToken->getString();
    });
}

const Token *TableSignature::getTokenPointer(const QString &token) const
{
    for(int index = 0; index < tokenTable.size(); index++)
    {
        if(token == tokenTable[index]->getString())
        {
            return tokenTable[index].get();
        }
    }

    QString errorMsg = "This token (\"";
    errorMsg += token;
    errorMsg += "\") is absent from this signature! Which means that this token was not declared!";

    throw std::invalid_argument(errorMsg.toStdString().data());
}

void TableSignature::pushTokenPointerToTable(const Token &token)
{
    shared_ptr<Token> ptr;
    ptr.reset(token.getAllocatedClone());

    tokenTable.push_back(ptr);
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
        throw std::invalid_argument("This token is already present in the table!");
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
    else
    {
        for(int index = 0; index < tokenTable.size(); index++)
        {
            if(*tokenTable[index] != *other.tokenTable[index])
            {
                return false;
            }
        }

        return true;
    }
}

void TableSignature::serialize(QDataStream &stream) const
{
    stream << tokenTable;
}

void TableSignature::unserialize(QDataStream &stream)
{
    stream >> tokenTable;
}

QDataStream &operator <<(QDataStream &stream, const shared_ptr<Token> &token)
{
    stream << *token;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, shared_ptr<Token> &token)
{
    token = Token::unserializePtr(stream);
    return stream;
}
