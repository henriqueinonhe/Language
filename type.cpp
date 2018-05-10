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

void Type::parseCompositeType(TypeParsingTreeIterator iter, const unsigned int startingIndex) //FIXME
{
    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);
    unsigned int mainOperatorIndex;

    if(leftArgumentIsPrimitiveType(iter->getTypeString()))
    {
        mainOperatorIndex = 1;
    }
    else if(leftArgumentIsCompositeType(iter->getTypeString()))
    {
        mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(iter->getTypeString(),
                                                                         TypeToken("("),
                                                                         TypeToken(")")) + 1;
    }
    else if(leftArgumentIsProductType(iter->getTypeString()))
    {
        mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(iter->getTypeString(),
                                                                         TypeToken("["),
                                                                         TypeToken("]")) + 1;
    }
    else
    {
        throw std::invalid_argument("The first character of the left argument should be either '(' or '['.");
    }

    //Main Operator Check
    if(!isCompositionOperato(iter->getTypeString()))
    {
        throw std::invalid_argument("The token at the main operator position should be a composition operator (->) !");
    }

    unsigned int leftArgumentParenthesisPadding = iter->getTypeString()[0] == TypeToken("(") ? 1 : 0;
    unsigned int rightArgumentParenthesisPadding = iter->getTypeString()[mainOperatorIndex + 1] == TypeToken("(") ? 1 : 0;

    iter->appendChild(startingIndex + leftArgumentParenthesisPadding,
                      startingIndex + mainOperatorIndex - 1 - leftArgumentParenthesisPadding);
    iter->appendChild(startingIndex + mainOperatorIndex + 1 + rightArgumentParenthesisPadding,
                      startingIndex + iter->getTypeString().size() - 1 - rightArgumentParenthesisPadding);


    //NEW
    iter.goToChild(0);
    parseLeftSideArgument(iter, startingIndex);
    iter.goToParent();

    iter.goToChild(1);
    parseRightSideArgument(iter, startingIndex + mainOperatorIndex + 1 /*Where from? */ + rightArgumentParenthesisPadding);

    //OLD
    iter.goToChild(0);
    parseType(iter, startingIndex, leftArgumentIsProductType(iter->getTypeString()));
    iter.goToParent();

    iter.goToChild(1);
    parseType(iter, startingIndex + mainOperatorIndex + 1 + rightArgumentParenthesisPadding, false);
}

void Type::findLastTokenIndex(TypeTokenString typeString, unsigned int &mainOpIndex, unsigned int &lastTokenIndex)
{
    if(typeString[mainOpIndex] != TypeToken("->"))
    {
        throw std::invalid_argument("The composition operator was expected here!");
    }

    if(typeString[mainOpIndex + 1].getSort() == TypeToken::Sort::PrimitiveType)
    {
        lastTokenIndex = mainOpIndex + 1;
    }
    else if(typeString[mainOpIndex + 1] == TypeToken("("))
    {
        lastTokenIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(typeString,
                                                                     TypeToken("("),
                                                                     TypeToken(")"),
                                                                     mainOpIndex + 1) + 1;
    }
    else
    {
        throw std::invalid_argument("A primitive type or a complete type was expected here!");
    }
}

bool Type::typeIsEmpty(const TypeTokenString &typeString)
{
    return typeString.size() == 0;
}

bool Type::isCompositionOperator(const TypeTokenString &typeString, unsigned int index)
{
    return typeString[mainOperatorIndex].getSort() == TypeToken::Sort::CompositionOperator;
}

void Type::parseRightSideArgument(TypeParsingTreeIterator iter, const unsigned int startingIndex)
{
    if(typeIsEmpty(tokenString))
    {
        throw std::invalid_argument("The type cannot be empty!");
    }

    if(isPrimitiveType(iter->getTypeString()))
    {
        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);
        return;
    }
    else //If it is not primitive, it must be composite then!
    {
        parseCompositeType(iter, startingIndex);
    }
}

void Type::findMainOperatorOffset(const TypeTokenString &tokenString)
{
    if(tokenString[0].getSort() == TypeToken::Sort::LeftParenthesis)
    {
        mainOperatorOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                               TypeToken("("),
                                                                               TypeToken(")")) + 1;
    }
    else if(tokenString[0].getSort() == TypeToken::Sort::LeftSquareBracket)
    {
        mainOperatorOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                               TypeToken("["),
                                                                               TypeToken("]")) + 1;
    }
    else
    {
        mainOperatorOffset = 1;
    }

    return mainOperatorOffset;
}

bool Type::stringEndsBeforeMainOperator(const TypeTokenString &tokenString, const unsigned int mainOperatorIndex)
{
    return tokenString.size() == mainOperatorIndex;
}

bool Type::isCompositionRightSideArgumentSuitableFirstToken(const TypeToken &token)
{
    return token.getSort() == TypeToken::Sort::PrimitiveType ||
           token.getSort() == TypeToken::Sort::LeftParenthesis;
}

void Type::parseType(TypeParsingTreeIterator iter, const unsigned int beginIndex)
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
    else if(isCompositeType(tokenString))
    {
        unsigned int mainOperatorOffset = findMainOperatorOffset(tokenString);

        if(isCompositeType(tokenString))
        {
            iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);

            const unsigned int leftArgumentParenthesisPadding = tokenString[0].getSort() == TypeToken::Sort::LeftParenthesis ? 1 : 0;
            const unsigned int leftArgumentBeginIndex = beginIndex + leftArgumentParenthesisPadding;
            const unsigned int leftArgumentEndIndex = beginIndex + mainOperatorOffset - mainOperatorCompensation - leftArgumentParenthesisPadding;
            const unsigned int mainOperatorCompensation = 1;
            iter->appendChild(leftArgumentBeginIndex, leftArgumentEndIndex);

            const unsigned int rightArgumentBeginOffset = mainOperatorOffset + mainOperatorCompensation;
            if(!parenthesisMatch(tokenString, rightArgumentBeginOffset))
            {
                throw std::invalid_argument("Parenthesis on the right side argument don't match");
            }

            if(tokenString[rightArgumentBeginOffset].getSort() == TypeToken::Sort::LeftSquareBracket)
            {
                throw std::invalid_argument("There cannot be a product type on the right side argument of a composition!");
            }

            const unsigned int rightArgumentParenthesisPadding = tokenString[mainOperatorOffset + mainOperatorCompensation].getSort() == TypeToken::Sort::LeftParenthesis ? 1 : 0;
            const unsigned int rightArgumentBeginIndex = beginIndex + rightArgumentBeginOffset + rightArgumentParenthesisPadding;
            const unsigned int rightArgumentEndIndex = beginIndex + tokenString.size() - rightArgumentParenthesisPadding;
            iter->appendChild(rightArgumentBeginIndex, rightArgumentEndIndex);

            iter.goToChild(0);
            parseType(iter, leftArgumentBeginIndex);
            iter.goToParent();

            iter.goToChild(1);
            parseType(iter, rightArgumentBeginIndex);
        }
        else
        {

        }
    }
}

bool Type::isCompositeType(const TypeTokenString &tokenString)
{
    if(tokenString[0].getSort() == TypeToken::Sort::LeftParenthesis)
    {
        const unsigned int mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                                 TypeToken("("),
                                                                                                 TypeToken(")")) + 1;
        if(stringEndsBeforeMainOperator(tokenString, mainOperatorIndex))
        {
            throw std::invalid_argument("Expected composition operator (->), but found end of input.");
        }
        else if(!tokenString[mainOperatorIndex].getSort() == TypeToken::Sort::CompositionOperator)
        {
            throw std::invalid_argument("Expected composition operator (->), but found something else.");
        }
        else if()
        {

        }
        else if(tokenString[mainOperatorIndex + 1].getSort() == TypeToken::Sort::LeftSquareBracket)
        {
            throw std::invalid_argument("Right side composition argument cannot have a product type.");
        }
        else if(isCompositionRightSideArgumentSuitableFirstToken(tokenString[mainOperatorIndex + 1]))
        {
            throw std::invalid_argument("Expected a primitive type or a left parenthesis, but found something else.");
        }

    }
    else if(tokenString[0].getSort() == TypeToken::Sort::LeftSquareBracket)
    {
        const unsigned int mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                                 TypeToken("]"),
                                                                                                 TypeToken("]")) + 1;

        if(stringEndsBeforeMainOperator(tokenString, mainOperatorIndex))
        {
            return false; //Means it is a product type!
        }
        else if(!tokenString[mainOperatorIndex].getSort() == TypeToken::Sort::CompositionOperator)
        {
            throw std::invalid_argument("Expected composition operator (->), but found something else.");
        }
    }
    else if(tokenString[0].getSort() == TypeToken::Sort::PrimitiveType)
    {
        const unsigned int mainOperatorIndex = 1;

        if(!tokenString[mainOperatorIndex].getSort() == TypeToken::Sort::CompositionOperator)
        {
            throw std::invalid_argument("Expected composition operator (->), but found something else.");
        }
    }
    else
    {
        return false;
    }


}

bool Type::parenthesisMatch(const TypeTokenString &tokenString, const unsigned int startIndex)
{
    try
    {
        const unsigned int delimiterScopeEndIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                                      TypeToken("("),
                                                                                                      TypeToken(")"),
                                                                                                      startIndex);
    }
    catch(std::invalid_argument &e)
    {
        return false;
    }

    return true;
}


void Type::buildParsingTree(const QString &typeString)
{
    TypeTokenString tokenString(typeString);

    parsingTree.reset(new TypeParsingTree(tokenString)); //We must assign this way to avoid copying the tree (copy constructor)
    TypeParsingTreeIterator iter(parsingTree.get());

    const unsigned int startingIndex = 0;


    //NEW NEW
    if(typeIsEmpty(tokenString))
    {
        throw std::invalid_argument("The type cannot be empty!");
    }

    if(isPrimitiveType(iter->getTypeString()))
    {
        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);
        return;
    }
    else //If it is not primitive, it must be composite then!
    {
        unsigned int mainOperatorIndex;

        if(leftArgumentIsPrimitiveType(iter->getTypeString()))
        {
            mainOperatorIndex = 1;
        }
        else if(leftArgumentIsCompositeType(iter->getTypeString()))
        {
            mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(iter->getTypeString(),
                                                                             TypeToken("("),
                                                                             TypeToken(")")) + 1;
        }
        else if(leftArgumentIsProductType(iter->getTypeString()))
        {
            mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(iter->getTypeString(),
                                                                             TypeToken("["),
                                                                             TypeToken("]")) + 1;
        }
        else
        {
            throw std::invalid_argument("The first character of the left argument should be either '(' or '['.");
        }

        //Main Operator Check
        if(!isCompositionOperato(iter->getTypeString()))
        {
            throw std::invalid_argument("The token at the main operator position should be a composition operator (->) !");
        }
        else
        {
            iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);
        }

        unsigned int leftArgumentParenthesisPadding = iter->getTypeString()[0] == TypeToken("(") ? 1 : 0;
        unsigned int rightArgumentParenthesisPadding = iter->getTypeString()[mainOperatorIndex + 1] == TypeToken("(") ? 1 : 0;

        iter->appendChild(startingIndex + leftArgumentParenthesisPadding,
                          startingIndex + mainOperatorIndex - 1 - leftArgumentParenthesisPadding);
        iter->appendChild(startingIndex + mainOperatorIndex + 1 + rightArgumentParenthesisPadding,
                          startingIndex + iter->getTypeString().size() - 1 - rightArgumentParenthesisPadding);


        //NEW
        iter.goToChild(0);
        parseLeftSideArgument(iter, startingIndex);
        iter.goToParent();

        iter.goToChild(1);
        parseRightSideArgument(iter, startingIndex + mainOperatorIndex + 1 /*Where from? */ + rightArgumentParenthesisPadding);
    }

    //NEW
    parseRightSideArgument(iter, startingIndex);
}

bool Type::isPrimitiveType(const TypeTokenString &typeString) const
{
    return typeString.size() == 1 && typeString[0].getSort() == TypeToken::Sort::PrimitiveType;
}


