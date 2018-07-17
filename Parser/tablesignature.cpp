#include "tablesignature.h"
#include "signature.cpp" //This is so fucking shady...

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
