﻿#ifndef TYPETOKENSTRING_H
#define TYPETOKENSTRING_H

#include "typetoken.h"
#include "pool.h"
#include <QVector>
#include <QString>
#include <QLinkedList>
#include <QDataStream>

class TypeTokenString
{
public:
    TypeTokenString(); //NOTE Maybe this is not good!
    TypeTokenString(const QString &string);

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

private:
    static Pool<TypeToken> pool;

    TypeTokenString(const QVector<PoolRecordPointer<TypeToken>> &tokenList);

    void lexString(const QString &string);
    bool characterIsSeparator(const QString &c) const;

    QVector<PoolRecordPointer<TypeToken>> tokenList;

    void lexCompositionOperator(const QString &string, int &index);
    void lexPrimitiveTypeToken(const QString &string, int &index);

    friend QDataStream &operator <<(QDataStream &stream, const TypeTokenString &string);
    friend QDataStream &operator >>(QDataStream &stream, TypeTokenString &string);
};

QDataStream &operator <<(QDataStream &stream, const TypeTokenString &string);
QDataStream &operator >>(QDataStream &stream, TypeTokenString &string);

#endif // TYPETOKENSTRING_H
