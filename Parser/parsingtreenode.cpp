#include "parsingtreenode.h"
#include "ParsingTree.h"

ParsingTreeNode::ParsingTreeNode(ParsingTree *tree, ParsingTreeNode *parent, const QVector<unsigned int> &coordinates, const unsigned int BeginIndex, const unsigned int EndIndex) :
    tree(tree),
    parent(parent),
    coordinates(coordinates),
    beginIndex(BeginIndex),
    endIndex(EndIndex)
{
    updateTreeHeight();
}

void ParsingTreeNode::printNodeToString(QString &str) const
{
    /* There are five informations to be printed:
     * 1st - "(Parent Node Index, This Node Index)" According to coordinate system.
     * 2nd - The formula's type.
     * 3rd - The formula iself.
     * 4th - The Free Variable Set.
     * 5th - The Bound Variable Set. */

    str += "(";
    if(coordinates.size() >= 2)
    {
        const unsigned int zeroIndexCompensation = 1;
        const unsigned int fatherNodeCompensation = 1;
        str += QString::number(coordinates[coordinates.size() - zeroIndexCompensation - fatherNodeCompensation]);
    }
    if(!coordinates.isEmpty())
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

void ParsingTreeNode::updateTreeHeight()
{
    if(tree->height < getHeight())
    {
        tree->height = getHeight();
    }
}

void ParsingTreeNode::setType(const Type &value)
{
    type = value;
}

Type ParsingTreeNode::getType() const
{
    return type;
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
    return coordinates;
}

QString ParsingTreeNode::coordinatesToString() const
{
    QString coordinatesString;

    coordinatesString += "(";
    if(!this->coordinates.empty())
    {
        const unsigned int lastIndexCompensation = 1;
        std::for_each(this->coordinates.begin(), this->coordinates.end() - lastIndexCompensation, [&](unsigned int e) {
            coordinatesString += QString::number(e);
            coordinatesString += ",";
        });
        coordinatesString += QString::number(this->coordinates.back());
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
    return coordinates.size();
}

unsigned int ParsingTreeNode::getChildrenNumber() const
{
    return children.size();
}

unsigned int ParsingTreeNode::getOwnChildNumber() const
{
    return coordinates.back();
}

void ParsingTreeNode::appendChild(const unsigned int BeginIndex, const unsigned int EndIndex)
{
    QVector<unsigned int> coordinates = this->coordinates;
    coordinates.push_back(this->children.size());

    children.push_back(make_shared<ParsingTreeNode>(ParsingTreeNode(this->tree, this, coordinates, BeginIndex, EndIndex)));
}

