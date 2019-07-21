#ifndef CORETOKEN_H
#define CORETOKEN_H

#include "token.h"
#include "type.h"

class CoreToken : public Token
{
public:
    CoreToken() = delete;
    CoreToken &operator =(const CoreToken &other) = default;
    CoreToken &operator =(CoreToken &&) = delete;
    virtual ~CoreToken() = default;

    CoreToken(QDataStream &stream);
    CoreToken(const QString &string, const Type &type);

    Type getType() const;

    virtual QString tokenClass() const override;

    virtual unique_ptr<Token> getAllocatedClone() const override;

protected:
    CoreToken(const CoreToken &other) = default;
    CoreToken(CoreToken &&) noexcept = default;

    virtual void serialize(QDataStream &stream) const override;
    virtual bool isEqual(const Token &other) const override;

private:
    QString validateCoreTokenString(const QString &tokenString) const;

    Type type;

    friend Token *Token::deserializePtr(QDataStream &stream);
    friend QDataStream &operator <<(QDataStream &stream, const CoreToken &token);
};

QDataStream &operator <<(QDataStream &stream, const CoreToken &token);

#endif // CORETOKEN_H
