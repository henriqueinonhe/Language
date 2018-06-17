#include "punctuationtoken.h"

PunctuationToken::PunctuationToken(const QString &string) :
    Token(string)
{
    if(string != "(" &&
       string != ")")
    {
        throw std::invalid_argument("A punctuation token must be a left parenthesis or a right parenthesis!");
    }
    else
    {
        this->string = string;
    }
}

QString PunctuationToken::tokenClass() const
{
    return "PunctuationToken";
}

Token *PunctuationToken::getAllocatedClone() const
{
    return new PunctuationToken(*this);
}

bool PunctuationToken::isEqual(const Token &other) const
{
    return Token::isEqual(other);
}
