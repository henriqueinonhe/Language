#include "type.h"

bool Type::operator==(const Type &other) const
{
    return this->typeString == other.typeString;
}

bool Type::operator!=(const Type &other) const
{
    return !(*this == other);
}

QString Type::toString() const
{
    return typeString.toString();
}

Type Type::checkType(const QVector<Type> argumentsTypes) const
{

}

Type::Type() :
    typeString(TypeTokenString(""))
{

}

Type::Type(const TypeTokenString &typeString) :
    typeString(typeString)
{

}

void Type::setTypeString(const TypeTokenString &value)
{
    typeString = value;
}

