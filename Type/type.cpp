#include "type.h"
#include <QDataStream>
#include "typeparser.h"
#include "typeparsingtreeiterator.h"
#include "typeparsingtreeconstiterator.h"
#include <QVector>

Type::Type(QDataStream &stream) :
    parsingTree(make_unique<TypeParsingTree>(stream))
{
}

Type::Type(const QString &type) :
    parsingTree(make_unique<TypeParsingTree>(TypeParser::getParsingTree(type)))
{
}

Type::Type(const Type &other) :
    parsingTree(make_unique<TypeParsingTree>(other.getParsingTree()))
{
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

Type Type::applyArguments(const QVector<Type> &argumentsTypes) const
{
    if(argumentsTypes == this->getArgumentsTypes())
    {
        return Type(getReturnType());
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

Type::Type(const TypeTokenString &typeString) :
    parsingTree(new TypeParsingTree(TypeParser::getParsingTree(typeString.toString())))
{

}

Type &Type::operator =(const Type &other)
{
    this->parsingTree.reset(new TypeParsingTree(other.getParsingTree()));
    return *this;
}

Type Type::getReturnType() const
{
    TypeParsingTreeConstIterator iter(parsingTree.get());
    if(iter->getMainOperator() == TypeParsingTreeNode::MainOperator::Primitive)
    {
        return Type(iter->getTypeString());
    }
    else //if(iter->getMainOperator() == TypeParsingTreeNode::MainOperator::Composition)
    {
        iter.goToChild(1);
        return Type(iter->getTypeString());
    }
}

TypeParsingTree Type::getParsingTree() const
{
    return *parsingTree;
}

unsigned int Type::getNumberOfArguments() const
{
    return static_cast<unsigned int>(getArgumentsTypes().size());
}

bool Type::isOperator() const
{
    return !getArgumentsTypes().isEmpty();
}

QVector<Type> Type::getArgumentsTypes() const
{
    TypeParsingTreeConstIterator iter(parsingTree.get());
    if(iter->getMainOperator() == TypeParsingTreeNode::MainOperator::Primitive)
    {
        return QVector<Type>();
    }
    else //if(iter->getMainOperator() == TypeParsingTreeNode::MainOperator::Composition)
    {
        QVector<Type> argumentsTypes;
        iter.goToChild(0);
        if(iter->getMainOperator() == TypeParsingTreeNode::MainOperator::Primitive)
        {
            argumentsTypes.push_back(Type(iter->getTypeString()));
        }
        else
        {
            for(unsigned int childNumber = 0; childNumber < iter->getChildrenNumber(); childNumber++)
            {
                iter.goToChild(childNumber);
                argumentsTypes.push_back(iter->getTypeString());
                iter.goToParent();
            }
        }
        return argumentsTypes;
    }
}

TypeTokenString Type::getTypeString() const
{
    return parsingTree->getTypeString();
}


QDataStream &operator <<(QDataStream &stream, const Type &type)
{
    stream << *type.parsingTree;
    return stream;
}

