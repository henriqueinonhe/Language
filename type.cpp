#include "type.h"

Type::Type(const QString &type)
{
    //Tá, aqui começa o parsing de verdade!
    //Quando cria o tipo ele constrói a árvore
    //e se conseguir terminar de construir
    //é porque tá bem formado.
    //Se não ele manda uma exceção!

    buildParsingTree(type);



}

void Type::buildParsingTree(const QString &typeString)
{
    TypeTokenString tokenString(typeString);

    parsingTree.reset(new TypeParsingTree(tokenString)); //É assim que tem que assinalar essa porra!

    TypeParsingTreeIterator iter(parsingTree.get());

    /* Antes de tudo checar se está vazio. */

    if(tokenString.size() == 0)
    {
        throw std::invalid_argument("The type cannot be empty!");
    }


    /* A primeira coisa a se fazer aqui então é
     * checar se a expressão corresponde a um tipo primitivo
     * ou não. */

    if(isPrimitive(iter->getTypeString()))
    {
        return; //Pode retornar pq a string ja vai tar no root e a árvore vai estar pronta!
    }
    else //Se ele não for primitivo, então deve necessariamente ser composto
    {
        unsigned int mainOperatorIndex;
        bool argumentIsIncompleteType;

        iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);

        if(iter->getTypeString()[0] == TypeToken("("))
        {
            mainOperatorIndex = ParsingAuxiliaryTools::findMainOperatorIndex(iter->getTypeString(),
                                                                             TypeToken("("),
                                                                             TypeToken(")"));

            argumentIsIncompleteType = false;
        }
        else if(iter->getTypeString()[0] == TypeToken("["))
        {
            mainOperatorIndex = ParsingAuxiliaryTools::findMainOperatorIndex(iter->getTypeString(),
                                                                             TypeToken("["),
                                                                             TypeToken("]"));

            argumentIsIncompleteType = true;
        }
        else
        {
            throw std::invalid_argument("The first character of the left argument should be either '(' or '['.");
        }

        iter->appendChild(0, mainOperatorIndex - 1);
        iter->appendChild(mainOperatorIndex + 1, iter->getTypeString().size() - 1);

        iter.goToChild(0);
        parseType(iter->getTypeString(), 0, iter, argumentIsIncompleteType);
        iter.goToParent();

        iter.goToChild(1);
        parseType(iter->getTypeString(), mainOperatorIndex + 1, iter, false); //Right argument is never an incomplete type

    }
}

bool Type::isPrimitive(const TypeTokenString &typeString) const
{
    return typeString.size() == 1 && typeString[0].getSort() == TypeToken::Sort::PrimitiveType;
}

void Type::parseType(const TypeTokenString &typeString, const unsigned int startingIndex, TypeParsingTreeIterator iter, const bool argumentIsIncompleteType) const
{

}

