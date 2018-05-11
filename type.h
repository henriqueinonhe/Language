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

    void parseType(TypeParsingTreeIterator iter);
    bool stringEndsBeforeMainOperator(const TypeTokenString &tokenString, const unsigned int mainOperatorIndex);
    bool isCompositionRightSideArgumentSuitableFirstToken(const TypeToken &token);

    //NEW
    bool hasProductTypeForm(const TypeTokenString &typeString) const;
    bool isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount) const;
    bool isProductArgumentBreakPoint(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token) const;
    bool isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount) const;
    void separateProductArguments(const TypeTokenString &tokenString, QVector<ProductArgumentOffsets> &offsetList) const;
    bool possiblyHasCompositeTypeForm(const TypeTokenString &typeString) const;


    struct ProductArgumentOffsets
    {
        ProductArgumentOffsets(const unsigned int beginOffset,
                               const unsigned int endOffset) :
            beginOffset(beginOffset),
            endOffset(endOffset)
        {
        }

        unsigned int beginOffset;
        unsigned int endOffset;
    };

};

#endif // TOKENTYPE_H
