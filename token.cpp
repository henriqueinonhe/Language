#include "token.h"

Token::Token()
{

}

Token::Token(QString string) :
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
