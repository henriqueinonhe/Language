#ifndef PUNCTUATIONTOKEN_H
#define PUNCTUATIONTOKEN_H

#include "token.h"

class PunctuationToken : public Token
{
public:
    PunctuationToken(const QString &string);

    virtual QString tokenClass() const;

    virtual Token *allocatedClone() const;

protected:
    virtual bool isEqual(const Token &other) const;
};

#endif // PUNCTUATIONTOKEN_H
