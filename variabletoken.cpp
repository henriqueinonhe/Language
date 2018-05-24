﻿#include "variabletoken.h"

VariableToken::VariableToken(const QString &token, const Type &type) :
    CoreToken(token, type)
{

}

QString VariableToken::tokenClass() const
{
    return "VariableToken";
}

Token *VariableToken::allocatedClone() const
{
    return new VariableToken(*this);
}

bool VariableToken::isEqual(const Token &other) const
{
    return CoreToken::isEqual(other);
}
