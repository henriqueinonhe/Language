#include "punctuationtoken.h"

PunctuationToken::PunctuationToken(QDataStream &stream) :
    Token(stream)
{
}

PunctuationToken::PunctuationToken(const QString &string) :
    Token(validatePunctuationTokenString(string))
{

}

QString PunctuationToken::tokenClass() const
{
    return "PunctuationToken";
}

unique_ptr<Token> PunctuationToken::getAllocatedClone() const
{
    return unique_ptr<Token>(new PunctuationToken(*this));
}

QString PunctuationToken::validatePunctuationTokenString(const QString &string) const
{
    if(string != "(" &&
       string != ")")
    {
        throw std::invalid_argument("A punctuation token must be a left parenthesis or a right parenthesis!");
    }
    return string;
}

bool PunctuationToken::isEqual(const Token &other) const
{
    return Token::isEqual(other) &&
           this->tokenClass() == other.tokenClass();
}

