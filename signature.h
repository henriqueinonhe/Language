#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "coretoken.h"
#include "type.h"

class Signature
{
public:
    Signature();
    virtual Token *getTokenPointer(const QString &token) = 0;

protected:
};

#endif // SIGNATURE_H
