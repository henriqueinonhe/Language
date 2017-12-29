#include "treenode.h"
#include "tree.h"

TreeNode::TreeNode(Tree *tree, TreeNode *parent, const QVector<unsigned int> &coordinates) :
    tree(tree),
    parent(parent),
    coordinates(coordinates)
{
    updateTreeHeight();
}

void TreeNode::updateTreeHeight()
{
    if(tree->height < getHeight())
    {
        tree->height = getHeight();
    }
}

QVector<unsigned int> TreeNode::getCoordinates() const
{
    return coordinates;
}

QString TreeNode::coordinatesToString() const
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

bool TreeNode::isRoot() const
{
    return parent == nullptr;
}

unsigned int TreeNode::getHeight() const
{
    return coordinates.size();
}

void TreeNode::appendChild()
{
    QVector<unsigned int> coordinates = this->coordinates;
    coordinates.push_back(this->children.size());

    children.push_back(make_shared<TreeNode>(TreeNode(this->tree, this, coordinates)));
}
