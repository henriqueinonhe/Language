#include "token.h"

Token::Token(const QString &string) :
    string(string)
{

}

bool Token::isEqual(const Token &other) const
{
    return this->string == other.string;
}

QString Token::getString() const
{
    return string;
}

bool Token::operator==(const Token &other) const
{
    if(typeid(*this) != typeid(other))
    {
        return false;
    }
    else
    {
        return this->isEqual(other);
    }
}

bool Token::operator!=(const Token &other) const
{
    return !(other == *this);
}

unsigned int Token::getTokenCharSpan() const
{
    return string.size();
}

QString Token::tokenClass() const
{
    return "Token";
}

Token *Token::getAllocatedClone() const
{
    return new Token(*this);
}
