﻿#include "basicprocessortokenrecord.h"

BasicProcessorTokenRecord::BasicProcessorTokenRecord()
{

}

BasicProcessorTokenRecord::BasicProcessorTokenRecord(const CoreToken &token, const unsigned int operatorPosition, const BasicProcessorTokenRecord::Associativity associativity) :
    token(token.getString()),
    operatorPosition(operatorPosition),
    numberOfArguments(token.getType().getNumberOfArguments()),
    associativity(associativity)
{
    /* Validation here basically consists of:
     * 1 - Token should be an operator, therefore it should take at least one argument!
     * 2 - Operator position should be consistent with the number of arguments the operator takes! */

    if(!token.getType().isOperator())
    {
        throw std::invalid_argument("The token associated with the record should be an operator!");
    }

    if(operatorPosition > numberOfArguments)
    {
        throw std::invalid_argument("The operator position is inconsistent with the number of arguments the operator takes!");
    }
}
