#include "variabletoken.h"

VariableToken::VariableToken(QDataStream &stream) :
    CoreToken(stream)
{
}

VariableToken::VariableToken(const QString &token, const Type &type) :
    CoreToken(token, type)
{

}

QString VariableToken::tokenClass() const
{
    return "VariableToken";
}

Token *VariableToken::getAllocatedClone() const
{
    return new VariableToken(*this);
}

bool VariableToken::isEqual(const Token &other) const
{
    return CoreToken::isEqual(other);
}
