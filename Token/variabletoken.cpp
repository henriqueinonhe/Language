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

unique_ptr<Token> VariableToken::getAllocatedClone() const
{
    return unique_ptr<Token>(new VariableToken(*this));
}

VariableToken::~VariableToken()
{

}

bool VariableToken::isEqual(const Token &other) const
{
    return CoreToken::isEqual(other);
}
