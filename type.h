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

    bool isPrimitiveType(const TypeTokenString &typeString) const;

    shared_ptr<TypeParsingTree> parsingTree;

    void findLastTokenIndex(TypeTokenString typeString, unsigned int &mainOpIndex, unsigned int &lastTokenIndex);

    bool typeIsEmpty(const TypeTokenString &typeString);

    //NEW
    void parseRightSideArgument(TypeParsingTreeIterator iter, const unsigned int startingIndex);

    //NEW NEW
    void parseType(TypeParsingTreeIterator iter, const unsigned int beginIndex);
    bool isCompositeType(const TypeTokenString &tokenString);
    bool parenthesisMatch(const TypeTokenString &tokenString, const unsigned int startIndex);
    void findMainOperatorOffset(const TypeTokenString &tokenString);
    bool stringEndsBeforeMainOperator(const TypeTokenString &tokenString, const unsigned int mainOperatorIndex);
    bool isCompositionRightSideArgumentSuitableFirstToken(const TypeToken &token);
};

#endif // TOKENTYPE_H
