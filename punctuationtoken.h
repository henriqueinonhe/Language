#ifndef PUNCTUATIONTOKEN_H
#define PUNCTUATIONTOKEN_H

#include "token.h"

class PunctuationToken : public Token
{
public:
    PunctuationToken(const QString &string);
};

#endif // PUNCTUATIONTOKEN_H
