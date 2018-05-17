#ifndef CORETOKEN_H
#define CORETOKEN_H

#include "token.h"
#include "type.h"
#include <QRegularExpression>

class CoreToken : public Token
{
public:
    CoreToken(const QString &string, const Type &type);

    bool operator ==(const CoreToken &other) const;
    bool operator !=(const CoreToken &other) const;

    Type getType() const;

private:
    Type type;

};

#endif // CORETOKEN_H
