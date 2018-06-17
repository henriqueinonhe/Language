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

protected:
    Type type;

    virtual bool isEqual(const Token &other) const;

};

#endif // CORETOKEN_H
