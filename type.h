#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <QSet>
#include <QVector>
#include <QString>
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "parserauxiliaryfunctions.h"
#include "typetokenstring.h"

class Type
{
public:
    Type(const QString &type);


private:
    TypeTokenString typeString;

};

#endif // TOKENTYPE_H
