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

/* Class Invariants:
 * Type uses a TypeParsingTree as an internal type representation
 * and this TypeParsingTree represents a valid type and is immutable. */

class Type
{
public:
    Type(QDataStream &stream);
    Type(const QString &type);
    Type(const Type &other);

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

    ~Type() = default;

private:
    Type();
    Type(const TypeTokenString &typeString);

    Type &operator =(const Type &other);

    unique_ptr<const TypeParsingTree> parsingTree;

    friend class QVector<Type>;
    friend class ParsingTreeNode; //TODO Remove this intrusion
    friend QDataStream &operator <<(QDataStream &stream, const Type &type);
};

QDataStream &operator <<(QDataStream &stream, const Type &type);

#endif // TOKENTYPE_H
