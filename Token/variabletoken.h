#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include "coretoken.h"

class VariableToken : public CoreToken
{
public:
    VariableToken(const QString &token, const Type &type);

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;

protected:
    VariableToken();

    virtual bool isEqual(const Token &other) const;

    friend shared_ptr<Token> Token::unserializePtr(QDataStream &stream);
};

#endif // VARIABLETOKEN_H
