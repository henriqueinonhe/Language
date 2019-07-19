#ifndef PUNCTUATIONTOKEN_H
#define PUNCTUATIONTOKEN_H

#include "token.h"

class PunctuationToken : public Token
{
public:
    PunctuationToken(QDataStream &stream);
    PunctuationToken(const QString &string);

    virtual QString tokenClass() const override;

    virtual unique_ptr<Token> getAllocatedClone() const override;

private:
    virtual bool isEqual(const Token &other) const override;

    friend Token *Token::deserializePtr(QDataStream &stream);
};

#endif // PUNCTUATIONTOKEN_H
