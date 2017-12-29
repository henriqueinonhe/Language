#include "typeparsingtreenode.h"
#include "TypeParsingTree.h"

TypeParsingTreeNode::TypeParsingTreeNode(TypeParsingTree *tree, TypeParsingTreeNode *parent, const QVector<unsigned int> &coordinates) :
    tree(tree),
    parent(parent),
    coordinates(coordinates)
{
    updateTreeHeight();
}

void TypeParsingTreeNode::updateTreeHeight()
{
    if(tree->height < getHeight())
    {
        tree->height = getHeight();
    }
}

QVector<unsigned int> TypeParsingTreeNode::getCoordinates() const
{
    return coordinates;
}

QString TypeParsingTreeNode::coordinatesToString() const
{
    QString coordinates;

    coordinates += "(";
    if(!this->coordinates.empty())
    {
        std::for_each(this->coordinates.begin(), this->coordinates.end() - 1, [&](unsigned int e) {
            coordinates += QString::number(e);
            coordinates += ",";
        });
        coordinates += QString::number(this->coordinates.back());
    }
    coordinates += ")";

    return coordinates;
}

bool TypeParsingTreeNode::isRoot() const
{
    return parent == nullptr;
}

TypeTokenString TypeParsingTreeNode::getTypeString() const
{
    return tree->typeString.mid(typeBeginIndex, typeEndIndex - typeBeginIndex + 1);
}

unsigned int TypeParsingTreeNode::getHeight() const
{
    return coordinates.size();
}

void TypeParsingTreeNode::appendChild()
{
    QVector<unsigned int> coordinates = this->coordinates;
    coordinates.push_back(this->children.size());

    children.push_back(make_shared<TypeParsingTreeNode>(TypeParsingTreeNode(this->tree, this, coordinates)));
}

