#include "coretoken.h"

CoreToken::CoreToken(const QString &string, const Type &type) :
    Token(string),
    type(type)
{
    if(!QRegularExpression("^([a-z]|[A-Z]|[0-9])+$").match(string).hasMatch())
    {
        throw std::invalid_argument("This is not a suitable Core Token!");
    }
}

bool CoreToken::operator ==(const CoreToken &other) const
{
    return this->string == other.string &&
           this->type == other.type;
}

bool CoreToken::operator !=(const CoreToken &other) const
{
    return !(*this == other);
}

Type CoreToken::getType() const
{
    return type;
}

QString CoreToken::tokenClass() const
{
    return "CoreToken";
}
