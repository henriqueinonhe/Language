#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <iostream>
#include <memory>
#include <QSet>
#include <QVector>
#include <QString>
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "parsingauxiliarytools.h"

using namespace std;

class Type
{
public:
    Type(const QString &type);

private:
    void buildParsingTree(const QString &typeString);

    bool isPrimitive(const TypeTokenString &typeString) const;
    void parseType(TypeParsingTreeIterator iter,
                   unsigned int startingIndex,
                   bool argumentIsIncompleteType);
    void parseCompositeType(TypeParsingTreeIterator iter, const unsigned int startingIndex);
    void parseUnionType(TypeParsingTreeIterator iter, const unsigned int startingIndex);

    shared_ptr<TypeParsingTree> parsingTree;

    void findLastTokenIndex(TypeTokenString typeString, unsigned int &mainOpIndex, unsigned int &lastTokenIndex);
};

#endif // TOKENTYPE_H
