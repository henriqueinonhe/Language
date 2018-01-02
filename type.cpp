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
    bool argumentIsIncompleteType;

    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);

    if(iter->getTypeString()[0] == TypeToken("("))
    {
        mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(iter->getTypeString(),
                                                                         TypeToken("("),
                                                                         TypeToken(")"));

        argumentIsIncompleteType = false;
    }
    else if(iter->getTypeString()[0] == TypeToken("["))
    {
        mainOperatorIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(iter->getTypeString(),
                                                                         TypeToken("["),
                                                                         TypeToken("]"));

        argumentIsIncompleteType = true;
    }
    else
    {
        throw std::invalid_argument("The first character of the left argument should be either '(' or '['.");
    }

    //Coloca os operandos do lado esquerdo e direito da composição.
    //Tirar os parênteses se necessário
    //Tomar muito cuidado com esses roles! //FIXME
    unsigned int leftArgumentParenthesisPadding = iter->getTypeString()[0] == TypeToken("(") ? 1 : 0;
    unsigned int rightArgumentParenthesisPadding = iter->getTypeString()[mainOperatorIndex + 1] == TypeToken("(") ? 1 : 0;

    iter->appendChild(startingIndex + leftArgumentParenthesisPadding,
                      startingIndex + mainOperatorIndex - 1 - leftArgumentParenthesisPadding);
    iter->appendChild(startingIndex + mainOperatorIndex + 1 + rightArgumentParenthesisPadding,
                      startingIndex + iter->getTypeString().size() - 1 - rightArgumentParenthesisPadding);



    iter.goToChild(0);
    parseType(iter, startingIndex, argumentIsIncompleteType);
    iter.goToParent();

    iter.goToChild(1);
    parseType(iter, startingIndex + mainOperatorIndex + 1, false);
}

void Type::findLastTokenIndex(TypeTokenString typeString, unsigned int &mainOpIndex, unsigned int &lastTokenIndex)
{
    if(typeString[mainOpIndex] != TypeToken("->"))
    {
        throw std::invalid_argument("The composite operator was expected here!");
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
                                                                     mainOpIndex + 1);
    }
    else
    {
        throw std::invalid_argument("A primitive type or a complete type was expected here!");
    }
}

void Type::parseUnionType(TypeParsingTreeIterator iter, const unsigned int startingIndex)
{
    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Union);

    TypeTokenString typeString = iter->getTypeString();

    unsigned int firstTokenIndex = 1;
    unsigned int lastTokenIndex;
    unsigned int childIndex = 0;

    while(true)
    {
        if(typeString[firstTokenIndex].getSort() == TypeToken::Sort::PrimitiveType)
        {
            lastTokenIndex = firstTokenIndex;
        }
        else if(typeString[firstTokenIndex] == TypeToken("["))
        {
            unsigned int mainOpIndex;
            mainOpIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(typeString,
                                                                       TypeToken("["),
                                                                       TypeToken("]"),
                                                                       firstTokenIndex);

            findLastTokenIndex(typeString, mainOpIndex, lastTokenIndex);
        }
        else if(typeString[firstTokenIndex] == TypeToken("("))
        {
            unsigned int mainOpIndex;
            mainOpIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(typeString,
                                                                       TypeToken("("),
                                                                       TypeToken(")"),
                                                                       firstTokenIndex);

            findLastTokenIndex(typeString, mainOpIndex, lastTokenIndex);
        }
        else
        {
            throw std::invalid_argument("This is a not a suitable token!");
        }


        unsigned int firstTokenTrueIndex = startingIndex + firstTokenIndex;
        unsigned int lastTokenTrueIndex = startingIndex + lastTokenIndex;
        iter->appendChild(firstTokenTrueIndex, lastTokenTrueIndex);
        iter.goToChild(childIndex);
        childIndex++;
        parseType(iter, firstTokenTrueIndex, false); //Will never be an incomplete type!
        iter.goToParent();

        if(typeString[lastTokenIndex + 1] == TypeToken(","))
        {
            firstTokenIndex = lastTokenIndex + 2;
        }
        else if(typeString[lastTokenIndex + 1] == TypeToken("}"))
        {
            break;
        }
        else
        {
            throw std::invalid_argument("A comma or a right curly bracket was expected here!");
        }
    }


}

void Type::buildParsingTree(const QString &typeString)
{
    TypeTokenString tokenString(typeString);

    if(tokenString.size() == 0)
    {
        throw std::invalid_argument("The type cannot be empty!");
    }

    parsingTree.reset(new TypeParsingTree(tokenString)); //We must assign this way to avoid copying the tree (copy constructor)
    TypeParsingTreeIterator iter(parsingTree.get());

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

void Type::parseType(TypeParsingTreeIterator iter, unsigned int startingIndex, bool argumentIsIncompleteType) //TODO Concertar as exceções, deixá-las mais informativas!
{
    TypeTokenString typeString = iter->getTypeString();

    if(typeString.size() == 0)
    {
        throw std::invalid_argument("Type cannot be empty!");
    }

    if(argumentIsIncompleteType)
    {
        unsigned int leftCurlyBracketIndex = 1;
        unsigned int rightCurlyBracketIndex;
        unsigned int leftCurlyBracketTrueIndex;
        unsigned int rightCurlyBracketTrueIndex;
        unsigned int childIndex = 0;

        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Product);

        while(true)
        {
            rightCurlyBracketIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(typeString,
                                                                                  TypeToken("{"),
                                                                                  TypeToken("}"),
                                                                                  leftCurlyBracketIndex) - 1;
            leftCurlyBracketTrueIndex = startingIndex + leftCurlyBracketIndex;
            rightCurlyBracketTrueIndex = startingIndex + rightCurlyBracketIndex;
            iter->appendChild(leftCurlyBracketTrueIndex, rightCurlyBracketTrueIndex);
            iter.goToChild(childIndex);
            parseUnionType(iter, leftCurlyBracketTrueIndex);

            if(typeString[rightCurlyBracketIndex + 1] == TypeToken(","))
            {
                leftCurlyBracketIndex = rightCurlyBracketIndex + 2;
            }
            else
            {
                if(typeString[rightCurlyBracketIndex + 1] == TypeToken("]"))
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

