#ifndef CORETOKEN_H
#define CORETOKEN_H

#include "token.h"
#include "type.h"
#include <QRegularExpression>

class CoreToken : public Token
{
public:
    CoreToken(const QString &string, const Type &type);

    Type getType() const;

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;

    virtual void serialize(QDataStream &stream) const;
    virtual void unserialize(QDataStream &stream);

protected:
    CoreToken();

    Type type;

    virtual bool isEqual(const Token &other) const;

    friend shared_ptr<Token> Token::unserializePtr(QDataStream &stream);
    friend QDataStream &operator <<(QDataStream &stream, const CoreToken &token);
    friend QDataStream &operator >>(QDataStream &stream, CoreToken &token);
};

QDataStream &operator <<(QDataStream &stream, const CoreToken &token);
QDataStream &operator >>(QDataStream &stream, CoreToken &token);

#endif // CORETOKEN_H
