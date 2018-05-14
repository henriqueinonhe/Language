#include "type.h"

Type::Type(const QString &type)
{
    //Tá, aqui começa o parsing de verdade!
    //Quando cria o tipo ele constrói a árvore
    //e se conseguir terminar de construir
    //é porque tá bem formado.
    //Se não ele manda uma exceção!

    buildParsingTree(type);

    std::cout << parsingTree->print().toStdString();

}

bool Type::operator==(const Type &other) const
{
    return *this->parsingTree == *other.parsingTree;
}

bool Type::operator!=(const Type &other) const
{
    return !(*this == other);
}

bool Type::typeIsEmpty(const TypeTokenString &typeString)
{
    return typeString.size() == 0;
}

bool Type::hasProductTypeForm(const TypeTokenString &typeString) const
{
    /* This is enough to settle the question whether the type in
     * question has the Product Type Form, because the right side
     * argument of a composition type cannot be a produc type,
     * therefore, the only case where the first and last characters
     * are square brackets are when the type is a product type. */

    return typeString.first().getSort() == TypeToken::Sort::LeftSquareBracket &&
           typeString.last().getSort() == TypeToken::Sort::RightSquareBracket;
}

bool Type::isWithinProductTypeMainScope(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount) const
{
    return leftSquareBracketCount == rightSquareBracketCount + 1;
}

bool Type::isProductArgumentBreakPoint(const unsigned int leftSquarteBracketCount, const unsigned int rightSquareBracketCount, const TypeToken &token) const
{
    return isWithinProductTypeMainScope(leftSquarteBracketCount, rightSquareBracketCount) &&
            token.getSort() == TypeToken::Sort::Comma;
}

bool Type::isProductArgumentsScopeEnd(const unsigned int leftSquareBracketCount, const unsigned int rightSquareBracketCount) const
{
    return leftSquareBracketCount == rightSquareBracketCount;
}

unsigned int Type::findCompositionOperatorOffset(const TypeTokenString &tokenString) const
{
    unsigned int mainOperatorOffset;
    const unsigned int mainOperatorCompensation = 1;

    if(tokenString.first().getSort() == TypeToken::Sort::PrimitiveType)
    {
        mainOperatorOffset = 1;
    }
    else if(tokenString.first().getSort() == TypeToken::Sort::LeftParenthesis)
    {
        mainOperatorOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                               TypeToken("("),
                                                                               TypeToken(")")) +
                                                                               mainOperatorCompensation;
    }
    else if(tokenString.first().getSort() == TypeToken::Sort::LeftSquareBracket)
    {
        mainOperatorOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                               TypeToken("["),
                                                                               TypeToken("]")) +
                                                                               mainOperatorCompensation;
    }
    else
    {
        throw ParsingErrorException<TypeTokenString>("This isn't a valid type! It is not a primitive type, nor a product type, nor a composite type (because the first token is neither a primitive type, a left parenthesis or a left square bracket).",
                                    0,
                                    0,
                                    tokenString);
    }

    if(!(tokenString[mainOperatorOffset].getSort() == TypeToken::Sort::CompositionOperator))
    {
        throw ParsingErrorException<TypeTokenString>("Composition operator (->) was expected here!",
                                        mainOperatorOffset,
                                        mainOperatorOffset,
                                        tokenString);
    }

    return mainOperatorOffset;
}

void Type::validateCompositionRightSideArgument(const TypeTokenString &tokenString, const unsigned int compositionOperatorOffset)
{
    const unsigned int tokenLookaheadCompensation = 1;
    if(tokenString[compositionOperatorOffset + tokenLookaheadCompensation].getSort() == TypeToken::Sort::PrimitiveType)
    {
        if(!tokenString.isLastIndex(compositionOperatorOffset + tokenLookaheadCompensation))
        {
            const unsigned int zeroIndexCompensation = 1;
            throw ParsingErrorException<TypeTokenString>("Right side argument of composition operator is a primtiive type, yet it has more tokens than it should!",
                                                         compositionOperatorOffset,
                                                         tokenString.size() - zeroIndexCompensation,
                                                         tokenString);
        }
    }
    else if(tokenString[compositionOperatorOffset + tokenLookaheadCompensation].getSort() == TypeToken::Sort::LeftParenthesis)
    {
        if(tokenString.last().getSort() != TypeToken::Sort::RightParenthesis)
        {
            const unsigned int zeroIndexCompensation = 1;
            throw ParsingErrorException<TypeTokenString>("There are uncased parenthesis in the right side argument of composition operator!",
                                                         compositionOperatorOffset,
                                                         tokenString.size() - zeroIndexCompensation,
                                                         tokenString);
        }
    }
    else
    {
        throw ParsingErrorException<TypeTokenString>("Right side argument of composition argument should begin with a left parenthesis or a primitive type.",
                                                     compositionOperatorOffset,
                                                     compositionOperatorOffset,
                                                     tokenString);
    }
}

void Type::separateProductArguments(const TypeTokenString &tokenString, QVector<ProductArgumentOffsets> &offsetList) const
{
    const unsigned int tokenLookaheadCompensation = 1;
    unsigned int leftSquareBracketCount = 1;
    unsigned int rightSquareBracketCount = 0;
    unsigned int argumentBeginOffset = 1;
    unsigned int argumentEndOffset = 1;

    while(true)
    {
        if(!tokenString.indexIsWithinBounds(argumentEndOffset))
        {

            //NOTE Will this ever happen?
            throw ParsingErrorException<TypeTokenString>("Type Token String ended before expected! Probably because the lack of a separator!",
                                        argumentBeginOffset,
                                        argumentEndOffset,
                                        tokenString);
        }

        if(tokenString[argumentEndOffset].getSort() == TypeToken::Sort::LeftSquareBracket)
        {
            leftSquareBracketCount++;
        }

        if(tokenString[argumentEndOffset].getSort() == TypeToken::Sort::RightSquareBracket)
        {
            rightSquareBracketCount++;
        }

        if(isProductArgumentBreakPoint(leftSquareBracketCount, rightSquareBracketCount, tokenString[argumentEndOffset]))
        {
            if(argumentEndOffset < argumentBeginOffset)
            {
                //NOTE Will this ever happen?
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
                const unsigned int zeroIndexCompensation = 1;
                throw ParsingErrorException<TypeTokenString>("There are unmatched square brackets!",
                                            0,
                                            tokenString.size() - zeroIndexCompensation,
                                            tokenString);
            }

            offsetList.last().endOffset = argumentEndOffset - tokenLookaheadCompensation;
            break;
        }

        argumentEndOffset++;
    }
}

void Type::parseProductType(const TypeTokenString &tokenString, TypeParsingTreeIterator &iter)
{
    QVector<ProductArgumentOffsets> offsetList = {ProductArgumentOffsets(1,1)};

    separateProductArguments(tokenString, offsetList);

    if(offsetList.size() == 1)
    {
        unsigned int zeroIndexCompensation = 1;

        throw ParsingErrorException<TypeTokenString>("Product types must have at least two argumets!",
                                        0,
                                        tokenString.size() - zeroIndexCompensation,
                                        tokenString);
    }

    std::for_each(offsetList.begin(), offsetList.end(), [&iter](const ProductArgumentOffsets &offsets)
    {
        const unsigned int argumentBeginIndex = iter->getTypeBeginIndex() + offsets.beginOffset;
        const unsigned int argumentEndIndex = iter->getTypeBeginIndex() + offsets.endOffset;

        iter->appendChild(argumentBeginIndex, argumentEndIndex);
    });

    for(unsigned int childIndex = 0; childIndex < iter->getChildrenNumber(); childIndex++)
    {
        iter.goToChild(childIndex);
        parseType(iter);
        iter.goToParent();
    }

    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Product);
}

void Type::parseCompositeType(TypeParsingTreeIterator &iter, const TypeTokenString &tokenString)
{
    unsigned int compositionOperatorOffset = findCompositionOperatorOffset(tokenString); //Also validates left side argument
    validateCompositionRightSideArgument(tokenString, compositionOperatorOffset);

    const unsigned int compositionOperatorCompensation = 1;
    const unsigned int zeroIndexCompensation = 1;

    const unsigned int leftArgumentParenthesisPadding = tokenString.first().getSort() == TypeToken::Sort::LeftParenthesis ? 1 : 0;
    const unsigned int leftArgumentBeginOffset = leftArgumentParenthesisPadding;
    const unsigned int leftArgumentEndOffset = compositionOperatorOffset - compositionOperatorCompensation - leftArgumentParenthesisPadding;

    const unsigned int rightArgumentParenthesisPadding = tokenString[compositionOperatorOffset + compositionOperatorCompensation].getSort() == TypeToken::Sort::LeftParenthesis ? 1 : 0;
    const unsigned int rightArgumentBeginOffset = compositionOperatorOffset + compositionOperatorCompensation + rightArgumentParenthesisPadding;
    const unsigned int rightArgumentEndOffset = tokenString.size() - zeroIndexCompensation - rightArgumentParenthesisPadding;

    const unsigned int leftArgumentBeginIndex = iter->getTypeBeginIndex() + leftArgumentBeginOffset;
    const unsigned int leftArgumentEndIndex = iter->getTypeBeginIndex() + leftArgumentEndOffset;
    const unsigned int rightArgumentBeginIndex = iter->getTypeBeginIndex() + rightArgumentBeginOffset;
    const unsigned int rightArgumentEndIndex = iter->getTypeBeginIndex() + rightArgumentEndOffset;

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

void Type::parseType(TypeParsingTreeIterator iter)
{
    const TypeTokenString tokenString = iter->getTypeString();

    if(typeIsEmpty(tokenString))
    {
        throw std::invalid_argument("The type cannot be empty!");
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
            parseProductType(tokenString, iter);
        }
        else
        {
            parseCompositeType(iter, tokenString);
        }
    }
}

void Type::buildParsingTree(const QString &typeString)
{
    TypeTokenString tokenString(typeString);

    parsingTree.reset(new TypeParsingTree(tokenString)); //We must assign this way to avoid copying the tree (copy constructor)
    TypeParsingTreeIterator iter(parsingTree.get());

    if(hasProductTypeForm(tokenString))
    {
        throw std::invalid_argument("The main type cannot be a product type!");
    }

    parseType(iter);
}

bool Type::isPrimitiveType(const TypeTokenString &typeString) const
{
    return typeString.size() == 1 && typeString.first().getSort() == TypeToken::Sort::PrimitiveType;
}


