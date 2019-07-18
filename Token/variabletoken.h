#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include "coretoken.h"

class VariableToken : public CoreToken
{
public:
    VariableToken(QDataStream &stream);
    VariableToken(const QString &token, const Type &type);

    virtual QString tokenClass() const override;

    virtual Token *getAllocatedClone() const override;

    virtual ~VariableToken() override;

protected:
    bool isEqual(const Token &other) const override;
    virtual bool isEqual(const VariableToken &other) const;

    friend Token *Token::deserializePtr(QDataStream &stream);
};

#endif // VARIABLETOKEN_H
