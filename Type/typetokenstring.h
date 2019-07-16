#ifndef TYPETOKENSTRING_H
#define TYPETOKENSTRING_H

#include "pool.h"
#include <QVector>
#include "typetoken.h"

class TypeToken;

class TypeTokenString
{
public:
    TypeTokenString() = default;
    TypeTokenString(QDataStream &stream);
    TypeTokenString(const QString &string);

    TypeTokenString(const TypeTokenString &) = default;
    TypeTokenString &operator =(const TypeTokenString &) = default;

    QString toString() const;

    unsigned int size() const;
    TypeTokenString mid(const unsigned int startPos, const unsigned int n) const;

    TypeToken operator[](unsigned int index) const;
    TypeToken first() const;
    TypeToken last() const;

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TypeTokenString &other) const;
    bool operator!=(const TypeTokenString &other) const;

    ~TypeTokenString() = default;

private:
    static Pool<TypeToken> pool;

    TypeTokenString(const QVector<PoolRecordPointer<TypeToken>> &tokenList);

    void lexString(const QString &string);
    bool characterIsSeparator(const QString &c) const;
    void lexCompositionOperator(const QString &string, int &index);
    void lexPrimitiveTypeToken(const QString &string, int &index);

    QVector<PoolRecordPointer<TypeToken>> tokenList;

    friend QDataStream &operator <<(QDataStream &stream, const TypeTokenString &string);
    friend QDataStream &operator >>(QDataStream &stream, TypeTokenString &string);
    friend class Type;
};

QDataStream &operator <<(QDataStream &stream, const TypeTokenString &string);
QDataStream &operator >>(QDataStream &stream, TypeTokenString &string);

#endif // TYPETOKENSTRING_H
