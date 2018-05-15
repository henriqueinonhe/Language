﻿#include "parsingtreenode.h"
#include "ParsingTree.h"

ParsingTreeNode::ParsingTreeNode(ParsingTree *tree, ParsingTreeNode *parent, const QVector<unsigned int> &coordinates, const unsigned int BeginIndex, const unsigned int EndIndex) :
    tree(tree),
    parent(parent),
    coordinates(coordinates),
    BeginIndex(BeginIndex),
    EndIndex(EndIndex)
{
    updateTreeHeight();
}

//void ParsingTreeNode::printNodeToString(QString &str) const //FIXME Rework Type Considerations
//{
//    str += "(";
//    if(coordinates.size() >= 2)
//    {
//        str += QString::number(coordinates[coordinates.size() - 2]);
//    }
//    if(!coordinates.isEmpty())
//    {
//        str += ",";
//        str += QString::number(coordinates.back());
//    }
//    str += "){";
//    str += mainOperatorToString();
//    str += ",\"";
//    str += getString().toString();
//    str += "\"} ";
//}

void ParsingTreeNode::updateTreeHeight()
{
    if(tree->height < getHeight())
    {
        tree->height = getHeight();
    }
}

unsigned int ParsingTreeNode::getEndIndex() const
{
    return EndIndex;
}

unsigned int ParsingTreeNode::getBeginIndex() const
{
    return BeginIndex;
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
        std::for_each(this->coordinates.begin(), this->coordinates.end() - 1, [&](unsigned int e) {
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

TokenString ParsingTreeNode::getString() const
{
    const unsigned int StringSize = EndIndex - BeginIndex + 1;
    return tree->tokenString.mid(BeginIndex, StringSize);
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

