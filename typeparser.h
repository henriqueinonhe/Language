#ifndef TYPEPARSER_H
#define TYPEPARSER_H

#include <iostream>
#include <memory>
#include <QVector>
#include <QString>
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "parsingauxiliarytools.h"
#include "parsingerrorexception.hpp"
#include "type.h"

class TypeParser
{
public:
    Type parse(const QString &type);

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
     bool isPrimitiveType(const TypeTokenString &typeString);
     bool hasProductTypeForm(const TypeTokenString &typeString);
     bool isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount);
     bool isProductArgumentBreakPoint(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token);
     bool isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount);

     void separateProductArguments(const TypeTokenString &tokenString, QVector<ProductArgumentOffsets> &offsetList);
     unsigned int findCompositionOperatorOffset(const TypeTokenString &tokenString);
     void validateCompositionRightSideArgument(const TypeTokenString &tokenString, const unsigned int compositionOperatorOffset);

    shared_ptr<TypeParsingTree> parsingTree;
};

#endif // TYPEPARSER_H
