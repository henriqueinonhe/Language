#include "typeparsingtreenode.h"
#include "TypeParsingTree.h"

TypeParsingTreeNode::TypeParsingTreeNode(TypeParsingTree *tree, TypeParsingTreeNode *parent, const QVector<unsigned int> &coordinates, const unsigned int typeBeginIndex, const unsigned int typeEndIndex, const MainOperator mainOperator) :
    tree(tree),
    parent(parent),
    coordinates(coordinates),
    typeBeginIndex(typeBeginIndex),
    typeEndIndex(typeEndIndex),
    mainOperator(mainOperator)
{
    updateTreeHeight();
}

void TypeParsingTreeNode::printNodeToString(QString &str) const //NOTE I really do not understand how this works!
{
    str += "(";
    if(coordinates.size() >= 2)
    {
        str += QString::number(coordinates[coordinates.size() - 2]);
    }
    if(!coordinates.isEmpty())
    {
        str += ",";
        str += QString::number(coordinates.back());
    }
    str += "){";
    str += mainOperatorToString();
    str += ",\"";
    str += getTypeString().toString();
    str += "\"} ";
}

QString TypeParsingTreeNode::mainOperatorToString() const
{
    if(mainOperator == MainOperator::Primitive)
    {
        return "()";
    }
    else if(mainOperator == MainOperator::Composition)
    {
        return "(->)";
    }
    else if(mainOperator == MainOperator::Product)
    {
        return "([])";
    }
    else if(mainOperator == MainOperator::Union)
    {
        return "({})";
    }
    else
    {
        throw std::logic_error("Main Operator is not set!");
        return "";
    }
}

void TypeParsingTreeNode::updateTreeHeight()
{
    if(tree->height < getHeight())
    {
        tree->height = getHeight();
    }
}

TypeParsingTreeNode::MainOperator TypeParsingTreeNode::getMainOperator() const
{
    return mainOperator;
}

void TypeParsingTreeNode::setMainOperator(const MainOperator &value)
{
    mainOperator = value;
}

QVector<unsigned int> TypeParsingTreeNode::getCoordinates() const
{
    return coordinates;
}

QString TypeParsingTreeNode::coordinatesToString() const
{
    QString coordinatesString;

    coordinatesString += "(";
    if(!this->coordinates.empty())
    {
        std::for_each(this->coordinates.begin(), this->coordinates.end() - 1, [&](unsigned int e) {
            coordinatesString += QString::number(e);
            coordinatesString += ",";
        });
        coordinatesString += QString::number(this->coordinates.back());
    }
    coordinatesString += ")";

    return coordinatesString;
}

bool TypeParsingTreeNode::isRoot() const
{
    return parent == nullptr;
}

bool TypeParsingTreeNode::isChildless() const
{
    return children.isEmpty();
}

TypeTokenString TypeParsingTreeNode::getTypeString() const
{
    const unsigned int typeStringSize = typeEndIndex - typeBeginIndex + 1;
    return tree->typeString.mid(typeBeginIndex, typeStringSize);
}

unsigned int TypeParsingTreeNode::getHeight() const
{
    return coordinates.size();
}

unsigned int TypeParsingTreeNode::getChildrenNumber() const
{
    return children.size();
}

unsigned int TypeParsingTreeNode::getOwnChildNumber() const
{
    return coordinates.back();
}

void TypeParsingTreeNode::appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex)
{
    QVector<unsigned int> coordinates = this->coordinates;
    coordinates.push_back(this->children.size());

    children.push_back(make_shared<TypeParsingTreeNode>(TypeParsingTreeNode(this->tree, this, coordinates, typeBeginIndex, typeEndIndex)));
}

