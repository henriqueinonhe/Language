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
#include "parsingerrorexception.hpp"

using namespace std;

class Type
{
public:
    Type(const QString &type);

    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;

private:
    struct ProductArgumentOffsets
    {
        ProductArgumentOffsets(const unsigned int beginOffset,
                               const unsigned int endOffset) :
            beginOffset(beginOffset),
            endOffset(endOffset)
        {
        }

        ProductArgumentOffsets(){}

        unsigned int beginOffset;
        unsigned int endOffset;
    };

    void buildParsingTree(const QString &typeString);
    void parseType(TypeParsingTreeIterator iter);
    void parseProductType(const TypeTokenString &tokenString, TypeParsingTreeIterator &iter);
    void parseCompositeType(TypeParsingTreeIterator &iter, const TypeTokenString &tokenString);

    bool typeIsEmpty(const TypeTokenString &typeString);
    bool isPrimitiveType(const TypeTokenString &typeString) const;
    bool hasProductTypeForm(const TypeTokenString &typeString) const;
    bool isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount) const;
    bool isProductArgumentBreakPoint(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token) const;
    bool isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount) const;

    void separateProductArguments(const TypeTokenString &tokenString, QVector<ProductArgumentOffsets> &offsetList) const;
    unsigned int findCompositionOperatorOffset(const TypeTokenString &tokenString) const;
    void validateCompositionRightSideArgument(const TypeTokenString &tokenString, const unsigned int compositionOperatorOffset);

    shared_ptr<TypeParsingTree> parsingTree;

};

#endif // TOKENTYPE_H
