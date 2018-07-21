#include "coretoken.h"

CoreToken::CoreToken(const QString &string, const Type &type) :
    Token(string),
    type(type)
{
    if(!QRegularExpression("^([a-z]|[A-Z]|[0-9]|\\+|\\-|\\\\|\\*|&|-|>|<|\\||=|\\~)+$").match(string).hasMatch())
    {
        throw std::invalid_argument("This is not a suitable Core Token!");
    }
}

Type CoreToken::getType() const
{
    return type;
}

QString CoreToken::tokenClass() const
{
    return "CoreToken";
}

Token *CoreToken::getAllocatedClone() const
{
    return new CoreToken(*this);
}

void CoreToken::serialize(QDataStream &stream) const
{
    Token::serialize(stream);
    stream << type;
}

void CoreToken::unserialize(QDataStream &stream)
{
    Token::unserialize(stream);
    stream >> type;
}

bool CoreToken::isEqual(const Token &other) const
{
    return Token::isEqual(other) &&
            this->type == dynamic_cast<const CoreToken &>(other).type;
}

CoreToken::CoreToken() :
    Token()
{

}

QDataStream &operator <<(QDataStream &stream, const CoreToken &token)
{
    token.serialize(stream);

    return stream;
}

QDataStream &operator >>(QDataStream &stream, CoreToken &token)
{
    token.unserialize(stream);

    return stream;
}
