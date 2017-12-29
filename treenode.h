#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QVector>
#include <memory>

class Tree;

using namespace std;

class TreeNode
{
public:
    void appendChild();

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;

    unsigned int getHeight() const;

private:
    TreeNode(Tree *tree, TreeNode *parent, const QVector<unsigned int> &coordinates);

    void updateTreeHeight();

    Tree *tree;
    TreeNode *parent;
    QVector<unsigned int> coordinates;
    QVector<shared_ptr<TreeNode>> children;

friend class TreeIterator;
friend class Tree;

};

#endif // TREENODE_H
