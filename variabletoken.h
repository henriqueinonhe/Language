#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include "coretoken.h"

class VariableToken : public CoreToken
{
public:
    VariableToken(const QString &token, const Type &type);

    virtual QString tokenClass() const;

    virtual Token *allocatedClone() const;

protected:
    virtual bool isEqual(const Token &other) const;
};

#endif // VARIABLETOKEN_H
