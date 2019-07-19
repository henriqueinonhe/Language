#ifndef PUNCTUATIONTOKEN_H
#define PUNCTUATIONTOKEN_H

#include "token.h"

class PunctuationToken : public Token
{
public:
    PunctuationToken() = delete;
    PunctuationToken(PunctuationToken &&) = default;
    PunctuationToken &operator =(const PunctuationToken &other) = delete;
    PunctuationToken &operator =(PunctuationToken &&) = default;
    virtual ~PunctuationToken() = default;

    PunctuationToken(QDataStream &stream);
    PunctuationToken(const QString &string);

    virtual QString tokenClass() const override;

    virtual unique_ptr<Token> getAllocatedClone() const override;

protected:
    PunctuationToken(const PunctuationToken&) = default;

private:
    virtual bool isEqual(const Token &other) const override;

    friend Token *Token::deserializePtr(QDataStream &stream);
};

#endif // PUNCTUATIONTOKEN_H
