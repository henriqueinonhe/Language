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
    BasicProcessorTokenRecord(const CoreToken &token,
                              const unsigned int operatorPosition,
                              const Associativity associativity);

private:
    QString token;
    unsigned int operatorPosition;
    unsigned int numberOfArguments;
    Associativity associativity;

    friend class BasicProcessor;
    friend class BasicPreProcessor;
    friend class BasicPostProcessor;
};

#endif // BASICPROCESSORTOKENRECORD_H
