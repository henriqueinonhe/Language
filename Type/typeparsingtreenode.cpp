#include "typeparsingtreenode.h"
#include "TypeParsingTree.h"

TypeParsingTreeNode::TypeParsingTreeNode(TypeParsingTree *tree, TypeParsingTreeNode *parent, const unsigned int typeBeginIndex, const unsigned int typeEndIndex, const MainOperator mainOperator) :
    tree(tree),
    parent(parent),
    typeBeginIndex(typeBeginIndex),
    typeEndIndex(typeEndIndex),
    mainOperator(mainOperator)
{
}

void TypeParsingTreeNode::printNodeToString(QString &str) const
{
    /* The printing convention consists on two things:
     * 1st - (Parent Node Index, This Node Index)
     * 2nd - The Type Sort. */

    const QVector<unsigned int> coordinates = getCoordinates();
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
    else
    {
        throw std::logic_error("Main Operator is not set!");
        return "";
    }
}

unsigned int TypeParsingTreeNode::getTypeEndIndex() const
{
    return typeEndIndex;
}

unsigned int TypeParsingTreeNode::getTypeBeginIndex() const
{
    return typeBeginIndex;
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
    QVector<unsigned int> coordinates;

    const TypeParsingTreeNode *ptr = this;
    while(!ptr->isRoot())
    {
        coordinates.prepend(ptr->getOwnChildNumber());
        ptr = ptr->parent;
    }

    return coordinates;
}

QString TypeParsingTreeNode::coordinatesToString() const
{
    QString coordinatesString;
    const QVector<unsigned int> coordinates = getCoordinates();

    coordinatesString += "(";
    if(!coordinates.empty())
    {
        std::for_each(coordinates.begin(), coordinates.end() - 1, [&](unsigned int e) {
            coordinatesString += QString::number(e);
            coordinatesString += ",";
        });
        coordinatesString += QString::number(coordinates.back());
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
    unsigned int height = 0;
    const TypeParsingTreeNode *ptr = this;
    while(!ptr->isRoot())
    {
        ptr = ptr->parent;
        height++;
    }

    return height;
}

unsigned int TypeParsingTreeNode::getChildrenNumber() const
{
    return children.size();
}

unsigned int TypeParsingTreeNode::getOwnChildNumber() const
{
    const TypeParsingTreeNode *ptr = parent;

    unsigned int ownChildNumber = 0;
    while(ptr->children[ownChildNumber].get() != this)
    {
        ownChildNumber++;
    }

    return ownChildNumber;
}

unsigned int TypeParsingTreeNode::getGreatestDescendantHeight() const
{
    if(this->isChildless())
    {
        return getHeight();
    }
    else
    {
        QVector<unsigned int> greatestDescendantHeightVector;

        std::for_each(children.begin(), children.end(), [&greatestDescendantHeightVector](const shared_ptr<TypeParsingTreeNode> &node)
        {
            greatestDescendantHeightVector.push_back(node->getGreatestDescendantHeight());
        });


        return *std::max_element(greatestDescendantHeightVector.begin(),
                                greatestDescendantHeightVector.end());
    }
}

void TypeParsingTreeNode::appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex)
{
    children.push_back(make_shared<TypeParsingTreeNode>(TypeParsingTreeNode(this->tree, this, typeBeginIndex, typeEndIndex)));
}

