#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include "typetokenstring.h"
#include "typeparsingtree.h"
#include <memory>

class TypeParser;
class ParsingTreeNode;
class TypeParsingTree;
class Token;
class CoreToken;

using namespace std;

class Type
{
public:
    Type(QDataStream &stream);
    Type(const QString &type);
    Type(const Type &other);

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

    QString toString() const;

    Type applyArguments(const QVector<TypeTokenString> &argumentsTypes) const;

    TypeTokenString getTypeString() const;

    QVector<TypeTokenString> getArgumentsTypes() const;
    TypeTokenString getReturnType() const;

    TypeParsingTree getParsingTree() const;

    unsigned int getNumberOfArguments() const;
    bool isOperator() const;

private:
    Type();
    Type(const TypeTokenString &typeString,
         const QVector<TypeTokenString> argumentsTypes,
         const TypeTokenString &returnTypeTokenString);

    Type &operator=(const Type &other); //This probably shouldn't exist!

    unique_ptr<TypeParsingTree> parsingTree;
    QVector<TypeTokenString> argumentsTypes;
    TypeTokenString returnTypeTokenString;

    friend class QVector<Type>;
    friend class TypeParser;
    friend class ParsingTreeNode;
    friend QDataStream &operator <<(QDataStream &stream, const Type &type);
};

QDataStream &operator <<(QDataStream &stream, const Type &type);

#endif // TOKENTYPE_H
