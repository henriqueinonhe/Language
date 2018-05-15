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
