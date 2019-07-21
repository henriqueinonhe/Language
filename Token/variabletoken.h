#ifndef VARIABLETOKEN_H
#define VARIABLETOKEN_H

#include "coretoken.h"

class VariableToken : public CoreToken
{
public:
    VariableToken() = delete;
    VariableToken &operator =(const VariableToken &) = delete;
    VariableToken &operator =(VariableToken &&) = delete;
    virtual ~VariableToken() override;

    VariableToken(QDataStream &stream);
    VariableToken(const QString &token, const Type &type);

    virtual QString tokenClass() const override;

    virtual unique_ptr<Token> getAllocatedClone() const override;
protected:
    VariableToken(const VariableToken &) = default;
    VariableToken(VariableToken &&) noexcept = default;

    virtual bool isEqual(const Token &other) const override;

    friend Token *Token::deserializePtr(QDataStream &stream);
};

#endif // VARIABLETOKEN_H
