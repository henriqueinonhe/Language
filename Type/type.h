#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include "typetokenstring.h"
#include <memory>
#include <QDataStream>

class TypeParser;
class ParsingTreeNode;
class TypeParsingTree;

using namespace std;

class Type
{
public:
    Type(const QString &type);

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

    QString toString() const;

    Type applyArguments(const QVector<TypeTokenString> &argumentsTypes) const;

    TypeTokenString getTypeString() const;

    QVector<TypeTokenString> getArgumentsTypes() const;
    TypeTokenString getReturnType() const;

    shared_ptr<TypeParsingTree> getParsingTree() const;

    unsigned int getNumberOfArguments() const;
    bool isOperator() const;

private:
    Type();
    Type(const TypeTokenString &typeString,
         const QVector<TypeTokenString> argumentsTypes,
         const TypeTokenString &returnTypeTokenString);

    void setTypeString(const TypeTokenString &value);

    TypeTokenString typeString;
    QVector<TypeTokenString> argumentsTypes;
    TypeTokenString returnTypeTokenString;

    friend class TypeParser;
    friend class ParsingTreeNode;

    friend QDataStream &operator <<(QDataStream &stream, const Type &type);
    friend QDataStream &operator >>(QDataStream &stream, Type &type);
};

QDataStream &operator <<(QDataStream &stream, const Type &type);
QDataStream &operator >>(QDataStream &stream, Type &type);

#endif // TOKENTYPE_H
