#ifndef BASICPROCESSORTOKENRECORD_H
#define BASICPROCESSORTOKENRECORD_H

#include "coretoken.h"

class BasicProcessor;
class BasicPreProcessor;
class BasicPostProcessor;

class BasicProcessorTokenRecord
{
public:
    enum class Associativity
    {
        Left,
        Right
    };

    BasicProcessorTokenRecord();
    BasicProcessorTokenRecord(CoreToken * const token,
                              const unsigned int operatorPosition,
                              const Associativity associativity);

private:
    CoreToken *token;
    unsigned int operatorPosition;
    Associativity associativity;

    friend class BasicProcessor;
    friend class BasicPreProcessor;
    friend class BasicPostProcessor;
};

#endif // BASICPROCESSORTOKENRECORD_H
