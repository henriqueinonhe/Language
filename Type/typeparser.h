#ifndef TYPEPARSER_H
#define TYPEPARSER_H
#include <memory>

using namespace std;

class TypeToken;
template <class T> class QVector;
class QString;
class TypeTokenString;
class Type;
class TypeParsingTree;
class TypeParsingTreeIterator;

class TypeParser
{
public:
    TypeParser() = delete;
    TypeParser(const TypeParser &) = delete;
    TypeParser &operator=(const TypeParser &) = delete;
    ~TypeParser() = delete;

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
    static TypeParsingTree getParsingTree(const QString &type);

    static void buildParsingTree(const TypeTokenString &typeString);
    static void parseType(TypeParsingTreeIterator iter);
    static void parseProductType(TypeParsingTreeIterator iter);
    static void parseCompositeType(TypeParsingTreeIterator iter);

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

    static TypeParsingTree *parsingTree; //For caching purposes

    friend class Type;
};

#endif // TYPEPARSER_H
