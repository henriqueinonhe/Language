#include "type.h"

bool Type::operator==(const Type &other) const
{
    return this->typeString == other.typeString;
}

bool Type::operator!=(const Type &other) const
{
    return !(*this == other);
}

Type::Type(const TypeTokenString &typeString) :
    typeString(typeString)
{

}

