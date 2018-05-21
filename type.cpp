#include "type.h"

#include "typeparser.h"

Type::Type(const QString &type)
{
    TypeParser::parse(TypeTokenString(type), this);
}

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
        Type newType;
        TypeParser::parse(returnType, &newType);

        return newType;
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

shared_ptr<TypeParsingTree> Type::getParsingTree() const
{
    return TypeParser::getParsingTree(this->toString());
}

QVector<TypeTokenString> Type::getArgumentsTypes() const
{
    return argumentsTypes;
}

TypeTokenString Type::getTypeString() const
{
    return typeString;
}

