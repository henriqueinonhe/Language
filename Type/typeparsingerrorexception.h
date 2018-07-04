#ifndef TYPEPARSINGERROREXCEPTION_H
#define TYPEPARSINGERROREXCEPTION_H

#include <stdexcept>
#include "typetokenstring.h"

class TypeParsingErrorException : public std::invalid_argument
{
public:
    TypeParsingErrorException(const char *str,
                              const unsigned int errorBeginOffset,
                              const unsigned int errorEndOffset,
                              const TypeTokenString &errorTokenString);


    unsigned int getErrorBeginOffset() const;
    unsigned int getErrorEndOffset() const;
    TypeTokenString getErrorTokenString() const;

private:
    unsigned int errorBeginOffset;
    unsigned int errorEndOffset;
    TypeTokenString errorTokenString;
};

#endif // TYPEPARSINGERROREXCEPTION_H
