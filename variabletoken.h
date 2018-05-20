#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include "coretoken.h"

class VariableToken : public CoreToken
{
public:
    VariableToken(const QString &token, const Type &type);

    virtual QString tokenClass() const;
};

#endif // VARIABLETOKEN_H
