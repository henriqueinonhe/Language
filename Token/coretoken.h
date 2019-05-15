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

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;


protected:
    virtual void serialize(QDataStream &stream) const;

    virtual bool isEqual(const Token &other) const;

    Type type;

    friend shared_ptr<Token> Token::unserializePtr(QDataStream &stream);
    friend QDataStream &operator <<(QDataStream &stream, const CoreToken &token);
};

QDataStream &operator <<(QDataStream &stream, const CoreToken &token);

#endif // CORETOKEN_H
