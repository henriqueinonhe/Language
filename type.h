#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include "typetokenstring.h"

class TypeParser;

using namespace std;

//TODO Implement pseudo-parsing and manipulation methods!

class Type
{
public:

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

private:
    Type(const TypeTokenString &typeString);

    TypeTokenString typeString;

    friend class TypeParser;

};

#endif // TOKENTYPE_H
