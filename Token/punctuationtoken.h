#ifndef PUNCTUATIONTOKEN_H
#define PUNCTUATIONTOKEN_H

#include "token.h"

class PunctuationToken : public Token
{
public:
    PunctuationToken(QDataStream &stream);
    PunctuationToken(const QString &string);

    virtual QString tokenClass() const override;

    virtual Token *getAllocatedClone() const override;

private:
    bool isEqual(const Token &other) const override;
    virtual bool isEqual(const PunctuationToken &other) const;

    friend Token *Token::deserializePtr(QDataStream &stream);
};

#endif // PUNCTUATIONTOKEN_H
