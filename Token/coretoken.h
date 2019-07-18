#ifndef CORETOKEN_H
#define CORETOKEN_H

#include "token.h"
#include "type.h"

class CoreToken : public Token
{
public:
    CoreToken(QDataStream &stream);
    CoreToken(const QString &string, const Type &type);

    Type getType() const;

    virtual QString tokenClass() const override;

    virtual Token *getAllocatedClone() const override;

    virtual ~CoreToken();

protected:
    virtual void serialize(QDataStream &stream) const override;
    virtual bool isEqual(const Token &other) const override;

private:
    Type type;

    friend Token *Token::deserializePtr(QDataStream &stream);
    friend QDataStream &operator <<(QDataStream &stream, const CoreToken &token);
};

QDataStream &operator <<(QDataStream &stream, const CoreToken &token);

#endif // CORETOKEN_H
