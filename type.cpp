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

void Type::parseCompositeType(TypeParsingTreeIterator iter, const unsigned int startingIndex)
{
    unsigned int mainOperatorIndex;

    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);

    if(leftArgumentIsCompositeType(iter->getTypeString()))
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

    unsigned int leftArgumentParenthesisPadding = iter->getTypeString()[0] == TypeToken("(") ? 1 : 0;
    unsigned int rightArgumentParenthesisPadding = iter->getTypeString()[mainOperatorIndex + 1] == TypeToken("(") ? 1 : 0;

    iter->appendChild(startingIndex + leftArgumentParenthesisPadding,
                      startingIndex + mainOperatorIndex - 1 - leftArgumentParenthesisPadding);
    iter->appendChild(startingIndex + mainOperatorIndex + 1 + rightArgumentParenthesisPadding,
                      startingIndex + iter->getTypeString().size() - 1 - rightArgumentParenthesisPadding);



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

bool Type::leftArgumentIsCompositeType(const TypeTokenString &typeString)
{
    return typeString[0] == TypeToken("(");
}

bool Type::leftArgumentIsProductType(const TypeTokenString &typeString)
{
    return typeString[0] == TypeToken("[");
}

void Type::parseRightSideArgument(TypeParsingTreeIterator iter, const unsigned int startingIndex)
{
    if(typeIsEmpty(tokenString))
    {
        throw std::invalid_argument("The type cannot be empty!");
    }

    if(isPrimitive(iter->getTypeString()))
    {
        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);
        return;
    }
    else //If it is not primitive, it must be composite then!
    {
        parseCompositeType(iter, 0);
    }
}

void Type::buildParsingTree(const QString &typeString)
{
    TypeTokenString tokenString(typeString);

    if(typeIsEmpty(tokenString))
    {
        throw std::invalid_argument("The type cannot be empty!");
    }

    parsingTree.reset(new TypeParsingTree(tokenString)); //We must assign this way to avoid copying the tree (copy constructor)
    TypeParsingTreeIterator iter(parsingTree.get());

    //NEW
    parseRightSideArgument(iter, 0);

    //OLD
    if(isPrimitive(iter->getTypeString()))
    {
        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);
        return;
    }
    else //If it is not primitive, it must be composite then!
    {
        parseCompositeType(iter, 0);
    }
}

bool Type::isPrimitive(const TypeTokenString &typeString) const
{
    return typeString.size() == 1 && typeString[0].getSort() == TypeToken::Sort::PrimitiveType;
}

void Type::parseType(TypeParsingTreeIterator iter, unsigned int startingIndex, bool argumentIsProductType) //TODO Concertar as exceções, deixá-las mais informativas!
{
    TypeTokenString typeString = iter->getTypeString();

    if(typeIsEmpty(typeString))
    {
        throw std::invalid_argument("Type cannot be empty!");
    }

    if(argumentIsProductType)
    {
        unsigned int leftParenthesisIndex = 1;
        unsigned int rightParenthesisIndex;
        unsigned int leftParenthesisTrueIndex;
        unsigned int rightParenthesisTrueIndex;
        unsigned int childIndex = 0;

        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Product);

        while(true)
        {
            rightParenthesisIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(typeString,
                                                                                  TypeToken("("),
                                                                                  TypeToken(")"),
                                                                                  leftParenthesisIndex); //Because there is no main Operator
            leftParenthesisTrueIndex = startingIndex + leftParenthesisIndex;
            rightParenthesisTrueIndex = startingIndex + rightParenthesisIndex;
            iter->appendChild(leftParenthesisTrueIndex, rightParenthesisTrueIndex);
            iter.goToChild(childIndex);
            parseUnionType(iter, leftParenthesisTrueIndex);

            if(typeString[rightParenthesisIndex + 1] == TypeToken(","))
            {
                leftParenthesisIndex = rightParenthesisIndex + 2;
            }
            else
            {
                if(typeString[rightParenthesisIndex + 1] == TypeToken("]"))
                {
                    break;
                }
                else
                {
                    throw std::invalid_argument("A comma (',') or a right square bracket (']') was expected here!");
                }
            }

            childIndex++;
            iter.goToParent();
        }
    }
    else
    {
        if(isPrimitive(typeString))
        {
            iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);
            return;
        }
        else //Se ele não for primitivo, então deve necessariamente ser composto
        {
            parseCompositeType(iter, startingIndex);
        }
    }
}

