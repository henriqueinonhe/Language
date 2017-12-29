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

    parsingTree = make_shared<TypeParsingTree>(TypeParsingTree(tokenString));

    /* A primeira coisa a se fazer aqui então é
     * checar se a expressão corresponde a um tipo primitivo
     * ou não. */

    if(isPrimitive(tokenString))
    {
        return;
    }



}

bool Type::isPrimitive(const TypeTokenString &typeString)
{
    return typeString.size() == 1 && typeString[0]->getSort() == TypeToken::Sort::PrimitiveType;
}

