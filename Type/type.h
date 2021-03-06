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
    Type(const Type &other);
    Type(Type &&) noexcept = default;
    Type &operator =(const Type &other);
    Type &operator =(Type &&) noexcept = default;
    ~Type() = default;

    Type(QDataStream &stream);
    Type(const QString &type);

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

    QString toString() const;

    Type applyArguments(const QVector<Type> &argumentsTypes) const;

    TypeTokenString getTypeString() const;

    QVector<Type> getArgumentsTypes() const;
    Type getReturnType() const;

    TypeParsingTree getParsingTree() const;

    unsigned int getNumberOfArguments() const;
    bool isOperator() const;

private:
    Type();
    Type(const TypeTokenString &typeString);

    unique_ptr<const TypeParsingTree> parsingTree;

    friend class QVector<Type>;
    friend class ParsingTreeNode; //TODO Try to remove this intrusion
    friend QDataStream &operator <<(QDataStream &stream, const Type &type);
};

QDataStream &operator <<(QDataStream &stream, const Type &type);

#endif // TOKENTYPE_H
