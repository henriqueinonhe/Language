#include "tablesignature.h"

TableSignature::TableSignature()
{

}

Token *TableSignature::getTokenPointer(const QString &token)
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

void TableSignature::addToken(const Token &token)
{
    const bool tokenAlreadyPresent = std::any_of(tokenTable.begin(), tokenTable.end(), [&token](const shared_ptr<Token> &tableToken)
    {
        return token == *tableToken;
    });

    if(tokenAlreadyPresent)
    {
        throw std::invalid_argument("This token is already present in the table!");
    }
    else
    {
        shared_ptr<Token> ptr;
        ptr.reset(token.allocatedClone());

        tokenTable.push_back(ptr);
    }
}
