#include "typeparsingtreenode.h"
#include "TypeParsingTree.h"

TypeParsingTreeNode::TypeParsingTreeNode(TypeParsingTree *tree, TypeParsingTreeNode *parent, const QVector<unsigned int> &coordinates, const unsigned int typeBeginIndex, const unsigned int typeEndIndex) :
    tree(tree),
    parent(parent),
    coordinates(coordinates),
    typeBeginIndex(typeBeginIndex),
    typeEndIndex(typeEndIndex)
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

void TypeParsingTreeNode::gatherChidrenStrings(QString &str) const
{
    std::for_each(children.begin(), children.end(), [&str](const shared_ptr<TypeParsingTreeNode> &node)
    {
        str += "(";
        str += QString::number(node->getCoordinates()[node->getCoordinates().size() - 2]);
        str += QString::number(node->getCoordinates().back());
        str += "){"
        str += node->getTypeString().toString();
        str += "} ";
    });
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

bool TypeParsingTreeNode::isChildless() const
{
    return children.isEmpty();
}

TypeTokenString TypeParsingTreeNode::getTypeString() const
{
    return tree->typeString.mid(typeBeginIndex, typeEndIndex - typeBeginIndex + 1);
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

