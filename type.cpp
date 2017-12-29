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
    /* A primeira coisa a se fazer aqui então é
     * checar se a expressão corresponde a um tipo primitivo
     * ou não. */
}

