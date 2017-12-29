#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <QSet>
#include <QVector>
#include <QString>
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "parserauxiliaryfunctions.h"

class Type
{
public:
    Type(const QString &type);

private:
    void buildParsingTree(const QString &typeString);


    TypeParsingTree parsingTree;

};

#endif // TOKENTYPE_H
