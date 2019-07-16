#include "typeparser.h"
#include <iostream>
#include "parsingauxiliarytools.h"
#include "parsingerrorexception.hpp"
#include "typeparsingtreeiterator.h"
#include "type.h"

TypeParsingTree *TypeParser::parsingTree = nullptr;

bool TypeParser::typeIsEmpty(const TypeTokenString &typeString)
{
    return typeString.size() == 0;
}

bool TypeParser::hasProductTypeForm(const TypeTokenString &typeString)
{
    /* This is enough to settle the question whether the type in
     * question has the Product Type Form, because the right side
     * argument of a composition type cannot be a produc type,
     * therefore, the only case where the first and last characters
     * are square brackets are when the type is a product type. */

    return typeString.first().sort() == TypeToken::Sort::LeftSquareBracket &&
           typeString.last().sort() == TypeToken::Sort::RightSquareBracket;
}

bool TypeParser::isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount)
{
    return leftSquareBracketCount == rightSquareBracketCount + 1;
}

bool TypeParser::isProductArgumentBreakPoint(const unsigned int leftSquarteBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token)
{
    return isWithinProductTypeMainScope(leftSquarteBracketCount, rightSquareBracketCount) &&
            token.sort() == TypeToken::Sort::Comma;
}

bool TypeParser::isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount)
{
    return leftSquareBracketCount == rightSquareBracketCount;
}

unsigned int TypeParser::findCompositionOperatorOffsetAndValidateLeftSideArgument(const TypeTokenString &tokenString, TypeParsingTreeIterator iter)
{
    /* This function ALSO validates left side argument.
     * The reason why this validation is not in a separate function is
     * because these two processes are tightly coupled toghether in such a way
     * that separating them would do more harm than good.
     *
     * It is called specifically when it is certain that it is
     * analyzing a composite type canditate and no other option is viable. */

    unsigned int compositionOperatorOffset;
    const auto compositionOperatorCompensation = 1;

    if(tokenString.first().sort() == TypeToken::Sort::PrimitiveType)
    {
        compositionOperatorOffset = 1;
    }
    else if(tokenString.first().sort() == TypeToken::Sort::LeftParenthesis)
    {
        try
        {
            compositionOperatorOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                   TypeToken("("),
                                                                                   TypeToken(")")) +
                                                                                   compositionOperatorCompensation;
        }
        catch(std::invalid_argument &exception)
        {
            throw ParsingErrorException<TypeTokenString>(exception.what(),
                                                         iter->getTypeBeginIndex(),
                                                         iter->getTypeEndIndex(),
                                                         iter.goToRoot()->getTypeString());
        }
    }
    else if(tokenString.first().sort() == TypeToken::Sort::LeftSquareBracket)
    {
        compositionOperatorOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                               TypeToken("["),
                                                                               TypeToken("]")) +
                                                                               compositionOperatorCompensation;
    }
    else
    {
        throw ParsingErrorException<TypeTokenString>("This isn't a valid type! It is not a primitive type, nor a product type, nor a composite type (because the first token is neither a primitive type, a left parenthesis or a left square bracket).",
                                    iter->getTypeBeginIndex(),
                                    iter->getTypeBeginIndex(),
                                    iter.goToRoot()->getTypeString());
    }

    if(!(tokenString[compositionOperatorOffset].sort() == TypeToken::Sort::CompositionOperator))
    {
        /* Even though it finds the composition operator index, it must also certify that the token that
         * occupies that index REALLY is a composition operator. */

        auto compositionOperatorIndex = iter->getTypeBeginIndex() + compositionOperatorOffset;
        throw ParsingErrorException<TypeTokenString>("Composition operator (->) was expected here!",
                                        compositionOperatorIndex,
                                        compositionOperatorIndex,
                                        iter.goToRoot()->getTypeString());
    }

    return compositionOperatorOffset;
}

void TypeParser::validateCompositionRightSideArgument(TypeParsingTreeIterator iter, const unsigned int compositionOperatorOffset)
{
    const auto tokenString = iter->getTypeString();
    const auto tokenLookaheadCompensation = 1;
    if(tokenString[compositionOperatorOffset + tokenLookaheadCompensation].sort() == TypeToken::Sort::PrimitiveType)
    {
        if(!tokenString.isLastIndex(compositionOperatorOffset + tokenLookaheadCompensation))
        {
            const auto zeroIndexCompensation = 1;
            throw ParsingErrorException<TypeTokenString>("Right side argument of composition operator is a primtiive type, yet it has more tokens than it should!",
                                                         compositionOperatorOffset,
                                                         tokenString.size() - zeroIndexCompensation,
                                                         tokenString);
        }
    }
    else if(tokenString[compositionOperatorOffset + tokenLookaheadCompensation].sort() == TypeToken::Sort::LeftParenthesis)
    {
        if(tokenString.last().sort() != TypeToken::Sort::RightParenthesis)
        {
            const auto zeroIndexCompensation = 1;
            throw ParsingErrorException<TypeTokenString>("There are uncased parenthesis in the right side argument of composition operator!",
                                                         compositionOperatorOffset,
                                                         tokenString.size() - zeroIndexCompensation,
                                                         tokenString);
        }
        else
        {
            const auto tokenLookaheadCompensation = 1;
            const auto argumentParenthesisPadding = 1;
            const auto argumentBeginIndex = compositionOperatorOffset + argumentParenthesisPadding + tokenLookaheadCompensation;
            const auto argumentSize = tokenString.size() - argumentBeginIndex - argumentParenthesisPadding;
            const auto rightSideArgument = tokenString.mid(argumentBeginIndex, argumentSize);
            findCompositionOperatorOffsetAndValidateLeftSideArgument(rightSideArgument, iter);
        }
    }
    else
    {
        throw ParsingErrorException<TypeTokenString>("Composition operator's right side argument should be either a composite type or a primitive type!",
                                                     compositionOperatorOffset,
                                                     compositionOperatorOffset,
                                                     tokenString);
    }
}

bool TypeParser::parsingTreeCacheCheck(const TypeParsingTree * const tree, const TypeTokenString &tokenString)
{
    return tree != nullptr && tree->getTypeString() == tokenString;
}

void TypeParser::separateProductArguments(TypeParsingTreeIterator iter, QVector<ProductArgumentOffsets> &offsetList)
{
    const auto tokenString = iter->getTypeString();
    const auto tokenLookaheadCompensation = 1;
    auto leftSquareBracketCount = 1;
    auto rightSquareBracketCount = 0;
    auto argumentBeginOffset = 1;
    auto argumentEndOffset = 1;

    while(true)
    {
        if(!tokenString.indexIsWithinBounds(argumentEndOffset))
        {
            const auto argumentBeginIndex = iter->getTypeBeginIndex() + argumentBeginOffset;
            const auto argumentEndIndex = iter->getTypeBeginIndex() + argumentEndOffset;
            throw ParsingErrorException<TypeTokenString>("Type Token String ended before expected!",
                                        argumentBeginIndex,
                                        argumentEndIndex,
                                        iter.goToRoot()->getTypeString());
        }

        if(tokenString[argumentEndOffset].sort() == TypeToken::Sort::LeftSquareBracket)
        {
            leftSquareBracketCount++;
        }

        if(tokenString[argumentEndOffset].sort() == TypeToken::Sort::RightSquareBracket)
        {
            rightSquareBracketCount++;
        }

        if(isProductArgumentBreakPoint(leftSquareBracketCount, rightSquareBracketCount, tokenString[argumentEndOffset]))
        {
            if(argumentEndOffset - tokenLookaheadCompensation < argumentBeginOffset)
            {
                throw std::invalid_argument("Argument End Offset cannot be less than Argument Begin Offset!");
            }

            offsetList.back().endOffset = argumentEndOffset - tokenLookaheadCompensation; //Current argument ended
            argumentEndOffset++;
            argumentBeginOffset = argumentEndOffset;
            offsetList.push_back(ProductArgumentOffsets(argumentBeginOffset, argumentBeginOffset)); //Next argument begun
        }

        if(isProductArgumentsScopeEnd(leftSquareBracketCount, rightSquareBracketCount))
        {
            if(!tokenString.isLastIndex(argumentEndOffset))
            {
                throw ParsingErrorException<TypeTokenString>("There are unmatched square brackets!",
                                            iter->getTypeBeginIndex(),
                                            iter->getTypeEndIndex(),
                                            iter.goToRoot()->getTypeString());
            }

            offsetList.last().endOffset = argumentEndOffset - tokenLookaheadCompensation;
            break;
        }

        argumentEndOffset++;
    }
}

void TypeParser::checkProductTypeHasAtLeastTwoArguments(const QVector<ProductArgumentOffsets> &offsetList, TypeParsingTreeIterator iter)
{
    if(offsetList.size() == 1)
    {
        throw ParsingErrorException<TypeTokenString>("Product types must have at least two arguments!",
                                        iter->getTypeBeginIndex(),
                                        iter->getTypeEndIndex(),
                                        iter.goToRoot()->getTypeString());
    }
}

void TypeParser::parseProductType(TypeParsingTreeIterator iter)
{
    const auto firstLeftSquareBracketCompensation = 1;
    QVector<ProductArgumentOffsets> offsetList = {ProductArgumentOffsets(firstLeftSquareBracketCompensation, firstLeftSquareBracketCompensation)};
    separateProductArguments(iter, offsetList);

    checkProductTypeHasAtLeastTwoArguments(offsetList, iter);

    for(const auto &offsets : offsetList)
    {
        const auto argumentBeginIndex = iter->getTypeBeginIndex() + offsets.beginOffset;
        const auto argumentEndIndex = iter->getTypeBeginIndex() + offsets.endOffset;

        iter->appendChild(argumentBeginIndex, argumentEndIndex);
    }

    for(unsigned int childIndex = 0; childIndex < iter->getChildrenNumber(); childIndex++)
    {
        iter.goToChild(childIndex);
        parseType(iter);
        iter.goToParent();
    }

    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Product);
}

void TypeParser::parseCompositeType(TypeParsingTreeIterator iter)
{
    const auto tokenString = iter->getTypeString();
    auto compositionOperatorOffset = findCompositionOperatorOffsetAndValidateLeftSideArgument(tokenString, iter);
    validateCompositionRightSideArgument(iter, compositionOperatorOffset);

    const auto compositionOperatorCompensation = 1;
    const auto zeroIndexCompensation = 1;

    const auto leftArgumentParenthesisPadding = tokenString.first().sort() == TypeToken::Sort::LeftParenthesis ? 1 : 0;
    const auto leftArgumentBeginOffset = leftArgumentParenthesisPadding;
    const auto leftArgumentEndOffset = compositionOperatorOffset - compositionOperatorCompensation - leftArgumentParenthesisPadding;

    const auto rightArgumentParenthesisPadding = tokenString[compositionOperatorOffset + compositionOperatorCompensation].sort() == TypeToken::Sort::LeftParenthesis ? 1 : 0;
    const auto rightArgumentBeginOffset = compositionOperatorOffset + compositionOperatorCompensation + rightArgumentParenthesisPadding;
    const auto rightArgumentEndOffset = tokenString.size() - zeroIndexCompensation - rightArgumentParenthesisPadding;

    const auto leftArgumentBeginIndex = iter->getTypeBeginIndex() + leftArgumentBeginOffset;
    const auto leftArgumentEndIndex = iter->getTypeBeginIndex() + leftArgumentEndOffset;
    const auto rightArgumentBeginIndex = iter->getTypeBeginIndex() + rightArgumentBeginOffset;
    const auto rightArgumentEndIndex = iter->getTypeBeginIndex() + rightArgumentEndOffset;

    iter->appendChild(leftArgumentBeginIndex, leftArgumentEndIndex);
    iter->appendChild(rightArgumentBeginIndex, rightArgumentEndIndex);

    iter.goToChild(0);
    parseType(iter);
    iter.goToParent();

    iter.goToChild(1);
    parseType(iter);
    iter.goToParent();

    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);
}


void TypeParser::parseType(TypeParsingTreeIterator iter)
{
    const auto tokenString = iter->getTypeString();

    if(typeIsEmpty(tokenString))
    {
        throw std::invalid_argument("Type cannot be empty!");
    }

    if(isPrimitiveType(tokenString))
    {
        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);
        return;
    }
    else
    {
        if(hasProductTypeForm(tokenString))
        {
            parseProductType(iter);
        }
        else
        {
            parseCompositeType(iter);
        }
    }
}

TypeParsingTree TypeParser::getParsingTree(const QString &type)
{
    buildParsingTree(type);

    return *parsingTree;
}

void TypeParser::buildParsingTree(const TypeTokenString &typeString)
{
    TypeTokenString tokenString(typeString);

    if(parsingTreeCacheCheck(parsingTree, tokenString))
    {
        return;
    }
    else
    {
        delete parsingTree;
        parsingTree = new TypeParsingTree(tokenString);
        TypeParsingTreeIterator iter(parsingTree);

        if(hasProductTypeForm(tokenString))
        {
            const auto zeroIndexCompensation = 1;
            throw ParsingErrorException<TypeTokenString>("The main type cannot be a product type!",
                                        0,
                                        tokenString.size() - zeroIndexCompensation,
                                        tokenString);
        }

        parseType(iter);
    }
}

bool TypeParser::isPrimitiveType(const TypeTokenString &typeString)
{
    return typeString.size() == 1 && typeString.first().sort() == TypeToken::Sort::PrimitiveType;
}
