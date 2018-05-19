#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include "typetokenstring.h"
#include <memory>

class TypeParser;
class ParsingTreeNode;

using namespace std;

//TODO Implement pseudo-parsing and manipulation methods!

class Type
{
public:

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

    QString toString() const;

    Type checkType(const QVector<TypeTokenString> &argumentsTypes) const;

    TypeTokenString getTypeString() const;

private:
    Type();
    Type(const TypeTokenString &typeString,
         const QVector<TypeTokenString> argumentsTypes,
         const TypeTokenString &returnType);

    void setTypeString(const TypeTokenString &value);

    TypeTokenString typeString;
    QVector<TypeTokenString> argumentsTypes;
    TypeTokenString returnType;

    friend class TypeParser;
    friend class ParsingTreeNode;
    friend class QVector<Type>;
};

#endif // TOKENTYPE_H
