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

Type Type::checkType(const QVector<TypeTokenString> &argumentsTypes) const
{
    if(argumentsTypes == this->argumentsTypes)
    {
        return Type(returnType, QVector<TypeTokenString>(), returnType);
    }
    else
    {
        throw std::invalid_argument("The types of the arguments don't match the types that this type takes!"); //FIXME
        //I need to intregrate this somehow with the parser itself, so messages become more informative!
    }
}

Type::Type()
{

}

Type::Type(const TypeTokenString &typeString, const QVector<TypeTokenString> argumentsTypes, const TypeTokenString &returnType) :
    typeString(typeString),
    argumentsTypes(argumentsTypes),
    returnType(returnType)
{

}

void Type::setTypeString(const TypeTokenString &value)
{
    typeString = value;
}

TypeTokenString Type::getReturnType() const
{
    return returnType;
}

QVector<TypeTokenString> Type::getArgumentsTypes() const
{
    return argumentsTypes;
}

TypeTokenString Type::getTypeString() const
{
    return typeString;
}

