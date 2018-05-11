#ifndef TYPETOKENSTRING_H
#define TYPETOKENSTRING_H

#include "typetoken.h"
#include "pool.h"
#include <QVector>
#include <QString>
#include <QLinkedList>

class TypeTokenString
{
public:
    TypeTokenString(const QString &string);

    QString toString() const;

    unsigned int size() const;
    TypeTokenString mid(const unsigned int startPos, const unsigned int n) const;

    TypeToken operator[](unsigned int index) const;
    TypeToken first() const; //TODO Test me
    TypeToken last() const; //TODO Test me

    bool isLastIndex(const unsigned int index) const; //TODO Test me
    bool indexIsWithinBounds(const unsigned int index) const; //TODO Test me

private:
    TypeTokenString(const QVector<PoolRecordPointer<TypeToken>> &tokenList);

    static Pool<TypeToken> pool;

    QVector<PoolRecordPointer<TypeToken>> tokenList;
    void lexString(const QString &string);
    bool characterIsSeparator(const QString &c) const;

};

#endif // TYPETOKENSTRING_H
