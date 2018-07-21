#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "coretoken.h"
#include "type.h"

class Signature
{
public:
    Signature();
    virtual const Token *getTokenPointer(const QString &token) const = 0;

};

Q_DECLARE_INTERFACE(Signature, "Signature")

#endif // SIGNATURE_H
