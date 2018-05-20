#include "variabletoken.h"

VariableToken::VariableToken(const QString &token, const Type &type) :
    CoreToken(token, type)
{

}

QString VariableToken::tokenClass() const
{
    return "VariableToken";
}
