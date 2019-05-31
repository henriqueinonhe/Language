#include "type.h"
#include <QDataStream>
#include "typeparser.h"

Type::Type(QDataStream &stream) :
    tree(TypeParser::getParsingTree()),
    returnTypeTokenString(stream)
{
    stream >> argumentsTypes;
}

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

Type Type::applyArguments(const QVector<TypeTokenString> &argumentsTypes) const
{
    if(argumentsTypes == this->argumentsTypes)
    {
        Type returnType;
        TypeParser::parse(returnTypeTokenString, &returnType);

        return returnType;
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
    returnTypeTokenString(returnType)
{

}

void Type::setTypeString(const TypeTokenString &value)
{
    typeString = value;
}

TypeTokenString Type::getReturnType() const
{
    return returnTypeTokenString;
}

TypeParsingTree Type::getParsingTree() const
{
    return tree;
}

unsigned int Type::getNumberOfArguments() const
{
    return static_cast<unsigned int>(argumentsTypes.size());
}

bool Type::isOperator() const
{
    return !argumentsTypes.isEmpty();
}

QVector<TypeTokenString> Type::getArgumentsTypes() const
{
    return argumentsTypes;
}

TypeTokenString Type::getTypeString() const
{
    return typeString;
}


QDataStream &operator <<(QDataStream &stream, const Type &type)
{
    stream << type.typeString << type.returnTypeTokenString << type.argumentsTypes;
    return stream;
}

