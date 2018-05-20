#include "token.h"

Token::Token(const QString &string) :
    string(string)
{

}

QString Token::getString() const
{
    return string;
}

bool Token::operator==(const Token &other) const
{
    return other.string == this->string;
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
