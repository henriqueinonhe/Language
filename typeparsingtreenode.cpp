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

void TypeParsingTreeNode::printChildren(QString &str, const bool isLastChild)
{
    if(children.isEmpty())
    {
        return;
    }

    str += "{";
    std::for_each(children.begin(), children.end() - 1, [&str](const shared_ptr<TypeParsingTreeNode> &ptr)
    {
        str += ptr->getTypeString().toString();
        str += ";";
    });

    str += children.back()->getTypeString().toString();
    str += "}   ";
    if(isLastChild)
    {
        str += "\n\n";
    }

    std::for_each(children.begin(), children.end() - 1, [&str](const shared_ptr<TypeParsingTreeNode> &ptr)
    {
        ptr->printChildren(str, false);
    });

    children.back()->printChildren(str, true);
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

void TypeParsingTreeNode::appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex)
{
    QVector<unsigned int> coordinates = this->coordinates;
    coordinates.push_back(this->children.size());

    children.push_back(make_shared<TypeParsingTreeNode>(TypeParsingTreeNode(this->tree, this, coordinates, typeBeginIndex, typeEndIndex)));
}

