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

    /* Antes de tudo checar se está vazio. */

    if(tokenString.size() == 0)
    {
        throw std::invalid_argument("The type cannot be empty!");
    }

    parsingTree.reset(new TypeParsingTree(tokenString)); //É assim que tem que assinalar essa porra!
    TypeParsingTreeIterator iter(parsingTree.get());

    /* A primeira coisa a se fazer aqui então é
     * checar se a expressão corresponde a um tipo primitivo
     * ou não. */

    if(isPrimitive(iter->getTypeString()))
    {
        return; //Pode retornar pq a string ja vai tar no root e a árvore vai estar pronta!
    }
    else //Se ele não for primitivo, então deve necessariamente ser composto
    {
        if(!isComposite(iter->getTypeString()))
        {
            //Se não for composto então tem que jogar uma exceção.
            //Aqui eu preciso criar uma classe exceção específica pra lidar com esses parsing problems!

            throw std::invalid_argument("ERROU!"); //TODO
        }
        else
        {
            //Se ele for composto de fato então aí eu posso decompor ele
            //em esquerda e direita.
            //Pra fazer isso eu preciso achar aonde está possivelmente o operador principal.

        }

    }



}

bool Type::isPrimitive(const TypeTokenString &typeString)
{
    return typeString.size() == 1 && typeString[0]->getSort() == TypeToken::Sort::PrimitiveType;
}

bool Type::isComposite(const TypeTokenString &typeString)
{

}

