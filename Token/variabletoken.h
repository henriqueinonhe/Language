#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include "coretoken.h"

class VariableToken : public CoreToken
{
public:
    VariableToken(QDataStream &stream);
    VariableToken(const QString &token, const Type &type);

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;

    virtual ~VariableToken();

protected:
    virtual bool isEqual(const Token &other) const;

    friend Token *Token::unserializePtr(QDataStream &stream);
};

#endif // VARIABLETOKEN_H
