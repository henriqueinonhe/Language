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
            return tokenTable[index];
        }
    }

    QString errorMsg = "This token (\"";
    errorMsg += token;
    errorMsg += "\") is absent from this signature! Which means that this token was not declared!";

    throw std::invalid_argument(errorMsg.toStdString().data());
}

void TableSignature::addToken(Token *token)
{
    const bool tokenAlreadyPresent = std::any_of(tokenTable.begin(), tokenTable.end(), [token](const Token *tableToken)
    {
        return *token == *tableToken;
    });

    if(tokenAlreadyPresent)
    {
        throw std::invalid_argument("This token is already present in the table!");
    }
    else
    {
        tokenTable.push_back(token);
    }
}
