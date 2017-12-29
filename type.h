#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <memory>
#include <QSet>
#include <QVector>
#include <QString>
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "parserauxiliaryfunctions.h"

using namespace std;

class Type
{
public:
    Type(const QString &type);

private:
    void buildParsingTree(const QString &typeString);

    bool isPrimitive(const TypeTokenString &typeString);

    shared_ptr<TypeParsingTree> parsingTree;

};

#endif // TOKENTYPE_H
