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
    static Type parse(const QString &type);
    static TypeParsingTree *getParsingTree(const QString &type);

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

     static void buildParsingTree(const QString &typeString);
     static void parseType(TypeParsingTreeIterator iter);
     static void parseProductType(const TypeTokenString &tokenString, TypeParsingTreeIterator &iter);
     static void parseCompositeType(TypeParsingTreeIterator &iter, const TypeTokenString &tokenString);
     static void setReturnAndArgumentsTypes(QVector<TypeTokenString> &argumentsTypes, TypeParsingTreeIterator &iter, TypeTokenString  &returnType);

     static bool typeIsEmpty(const TypeTokenString &typeString);
     static bool isPrimitiveType(const TypeTokenString &typeString);
     static bool hasProductTypeForm(const TypeTokenString &typeString);
     static bool isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount);
     static bool isProductArgumentBreakPoint(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token);
     static bool isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount);

     static void separateProductArguments(const TypeTokenString &tokenString, QVector<ProductArgumentOffsets> &offsetList);
     static unsigned int findCompositionOperatorOffset(const TypeTokenString &tokenString);
     static void validateCompositionRightSideArgument(const TypeTokenString &tokenString, const unsigned int compositionOperatorOffset);

     static bool parsingTreeCacheCheck(const TypeParsingTree * const tree, const TypeTokenString &tokenString);

     static TypeParsingTree *parsingTree;
};

#endif // TYPEPARSER_H
