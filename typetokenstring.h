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

private:
    static Pool<TypeToken> pool;

    QVector<PoolRecordPointer<TypeToken>> tokenList;
    void lexString(const QString &string);
    bool characterIsSeparator(const QString &c) const;

};

#endif // TYPETOKENSTRING_H
