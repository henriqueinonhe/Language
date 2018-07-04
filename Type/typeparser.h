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
#include "dirtyfix.h"

class TypeParser
{
public:

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
    static void parse(const TypeTokenString &type, Type *newType);
    static shared_ptr<TypeParsingTree> getParsingTree(const QString &type);

    static void buildParsingTree(const TypeTokenString &typeString);
    static void parseType(TypeParsingTreeIterator iter);
    static void parseProductType(TypeParsingTreeIterator iter);
    static void parseCompositeType(TypeParsingTreeIterator iter);
    static void setReturnAndArgumentsTypes(QVector<TypeTokenString> &argumentsTypes, TypeParsingTreeIterator &iter, TypeTokenString  &returnType);

    static bool typeIsEmpty(const TypeTokenString &typeString);
    static bool isPrimitiveType(const TypeTokenString &typeString);
    static bool hasProductTypeForm(const TypeTokenString &typeString);
    static bool isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount);
    static bool isProductArgumentBreakPoint(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token);
    static bool isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount);

    static void separateProductArguments(TypeParsingTreeIterator iter, QVector<ProductArgumentOffsets> &offsetList);
    static void checkProductTypeHasAtLeastTwoArguments(const QVector<ProductArgumentOffsets> &offsetList, TypeParsingTreeIterator iter);
    static unsigned int findCompositionOperatorOffsetAndValidateLeftSideArgument(const TypeTokenString &tokenString, TypeParsingTreeIterator iter);
    static void validateCompositionRightSideArgument(TypeParsingTreeIterator iter, const unsigned int compositionOperatorOffset);

    static bool parsingTreeCacheCheck(const TypeParsingTree * const tree, const TypeTokenString &tokenString);

    static shared_ptr<TypeParsingTree> parsingTree; //FIXME There is something wrong here! But I really don't know what it is!

    friend class Type;
    friend class DirtyFix;
};

#endif // TYPEPARSER_H
