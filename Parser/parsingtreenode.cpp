#include "parsingtreenode.h"
#include "parsingtree.h"
#include <QVector>
#include "tokenstring.h"
#include "variabletoken.h"

ParsingTreeNode::ParsingTreeNode(const ParsingTree *tree, const ParsingTreeNode *parent, const unsigned int BeginIndex, const unsigned int EndIndex) :
    tree(tree),
    parent(parent),
    beginIndex(BeginIndex),
    endIndex(EndIndex)
{
}

void ParsingTreeNode::copyValues(const ParsingTreeNode &other)
{
    beginIndex = other.beginIndex;
    endIndex = other.endIndex;
    type = other.type;
    freeVariables = other.freeVariables;
    boundVariables = other.boundVariables;
}

void ParsingTreeNode::printNodeToString(QString &str) const
{
    /* There are five informations to be printed:
     * 1st - "(Parent Node Index, This Node Index)" According to coordinate system.
     * 2nd - The formula's type.
     * 3rd - The formula iself.
     * 4th - The Free Variable Set.
     * 5th - The Bound Variable Set. */


    const unsigned int height = getHeight();
    const QVector<unsigned int> coordinates = getCoordinates();
    str += "(";
    if(height >= 2)
    {
        const unsigned int zeroIndexCompensation = 1;
        const unsigned int fatherNodeCompensation = 1;
        str += QString::number(coordinates[static_cast<int>(static_cast<unsigned int>(coordinates.size()) - zeroIndexCompensation - fatherNodeCompensation)]);
    }
    if(height != 0)
    {
        str += ",";
        str += QString::number(coordinates.back());
    }
    str += "){\"";
    str += type.toString();
    str += "\",\"";
    str += getTokenString().formattedString();
    str += "\",FV=";
    str += printVariableSet(freeVariables);
    str += ",BV=";
    str += printVariableSet(boundVariables);
    str += "} ";
}

QString ParsingTreeNode::printVariableSet(const QSet<const VariableToken *> &set) const
{
    if(set.isEmpty())
    {
        return "{}";
    }
    else
    {
        QString setString;
        setString += "{\"";

        QList<const VariableToken *> list = set.toList();
        const unsigned int lastIndexCompensation = 1;
        std::for_each(list.begin(), list.end() - lastIndexCompensation, [&setString](const VariableToken *token)
        {
            setString += token->getString();
            setString += "\",\"";
        });

        setString += list.last()->getString();
        setString += "\"}";

        return setString;
    }
}

Type ParsingTreeNode::getType() const
{
    return type;
}

void ParsingTreeNode::setType(const Type &value)
{
    type = value;
}

ParsingTreeNode::ParsingTreeNode(const ParsingTree *tree, const ParsingTreeNode *parent) :
    tree(tree),
    parent(parent)
{

}

unsigned int ParsingTreeNode::getEndIndex() const
{
    return endIndex;
}

unsigned int ParsingTreeNode::getBeginIndex() const
{
    return beginIndex;
}

QVector<unsigned int> ParsingTreeNode::getCoordinates() const
{
    QVector<unsigned int> coordinates;

    const ParsingTreeNode *ptr = this;
    while(!ptr->isRoot())
    {
        coordinates.prepend(ptr->getOwnChildNumber());
        ptr = ptr->parent;
    }

    return coordinates;
}

QString ParsingTreeNode::coordinatesToString() const
{
    QString coordinatesString;
    const QVector<unsigned int> coordinates = this->getCoordinates();

    coordinatesString += "(";
    if(!coordinates.empty())
    {
        const unsigned int lastIndexCompensation = 1;
        std::for_each(coordinates.begin(), coordinates.end() - lastIndexCompensation, [&](unsigned int e) {
            coordinatesString += QString::number(e);
            coordinatesString += ",";
        });
        coordinatesString += QString::number(coordinates.back());
    }
    coordinatesString += ")";

    return coordinatesString;
}

bool ParsingTreeNode::isRoot() const
{
    return parent == nullptr;
}

bool ParsingTreeNode::isChildless() const
{
    return children.isEmpty();
}

TokenString ParsingTreeNode::getTokenString() const
{
    const unsigned int zeroIndexCompensation = 1;
    const unsigned int stringSize = endIndex - beginIndex + zeroIndexCompensation;
    return tree->tokenString.mid(beginIndex, stringSize);
}

unsigned int ParsingTreeNode::getHeight() const
{
    unsigned int height = 0;
    const ParsingTreeNode *ptr = this;
    while(!ptr->isRoot())
    {
        ptr = ptr->parent;
        height++;
    }

    return height;
}

unsigned int ParsingTreeNode::getChildrenNumber() const
{
    return static_cast<uint>(children.size());
}

unsigned int ParsingTreeNode::getOwnChildNumber() const
{
    const ParsingTreeNode *ptr = parent;

    unsigned int ownChildNumber = 0;
    while(ptr->children[static_cast<int>(ownChildNumber)].get() != this)
    {
        ownChildNumber++;
    }

    return ownChildNumber;
}

unsigned int ParsingTreeNode::getGreatestDescendantHeight() const
{
    if(this->isChildless())
    {
        return getHeight();
    }
    else
    {
        QVector<unsigned int> greatestDescendantHeightVector;

        std::for_each(children.begin(), children.end(), [&greatestDescendantHeightVector](const shared_ptr<ParsingTreeNode> &node)
        {
            greatestDescendantHeightVector.push_back(node->getGreatestDescendantHeight());
        });


        return *std::max_element(greatestDescendantHeightVector.begin(),
                                greatestDescendantHeightVector.end());
    }
}

void ParsingTreeNode::appendChild(const unsigned int BeginIndex, const unsigned int EndIndex)
{
    children.push_back(make_shared<ParsingTreeNode>(ParsingTreeNode(this->tree, this, BeginIndex, EndIndex)));
}

