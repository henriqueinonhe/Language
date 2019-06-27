#include "type.h"
#include <QDataStream>
#include "typeparser.h"

Type::Type(QDataStream &stream) :
    parsingTree(new TypeParsingTree(stream)),
    returnTypeTokenString(stream)
{
    stream >> argumentsTypes;
}

Type::Type(const QString &type) :
    parsingTree(new TypeParsingTree(TypeParser::getParsingTree(type)))
{
    TypeParser::parse(TypeTokenString(type), this); //Why is this necessary?
}

Type::Type(const Type &other) :
    parsingTree(new TypeParsingTree(other.getParsingTree())),
    argumentsTypes(other.argumentsTypes),
    returnTypeTokenString(other.getReturnType())
{
}

Type &Type::operator=(const Type &other)
{
    this->parsingTree.reset(new TypeParsingTree(other.getParsingTree()));
    this->argumentsTypes = other.argumentsTypes;
    this->returnTypeTokenString = other.returnTypeTokenString;
    return *this;
}

bool Type::operator==(const Type &other) const
{
    return this->parsingTree->getTypeString() == other.parsingTree->getTypeString();
}

bool Type::operator!=(const Type &other) const
{
    return !(*this == other);
}

QString Type::toString() const
{
    return parsingTree->getTypeString().toString();
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

Type::Type() :
    parsingTree(nullptr)
{

}

Type::Type(const TypeTokenString &typeString, const QVector<TypeTokenString> argumentsTypes, const TypeTokenString &returnType) :
    parsingTree(new TypeParsingTree(TypeParser::getParsingTree(typeString.toString()))),
    argumentsTypes(argumentsTypes),
    returnTypeTokenString(returnType)
{

}

TypeTokenString Type::getReturnType() const
{
    return returnTypeTokenString;
}

TypeParsingTree Type::getParsingTree() const
{
    return *parsingTree;
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
    //FIXME!

    return argumentsTypes;
}

TypeTokenString Type::getTypeString() const
{
    //FIXME!
    return parsingTree->getTypeString();

}


QDataStream &operator <<(QDataStream &stream, const Type &type)
{
    stream << *type.parsingTree << type.returnTypeTokenString << type.argumentsTypes;
    return stream;
}

