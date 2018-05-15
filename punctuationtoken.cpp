#include "punctuationtoken.h"

PunctuationToken::PunctuationToken(const QString &string) :
    Token(string)
{
    if(string != "(" &&
       string != ")" &&
       string != ",")
    {
        throw std::invalid_argument("A punctuation token must be a left parenthesis, a right parenthesis or a comma!");
    }
    else
    {
        this->string = string;
    }
}
