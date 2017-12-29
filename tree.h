#ifndef TREE_H
#define TREE_H

#include <QString>
#include <memory>
#include "treenode.h"

using namespace std;

class Tree
{
public:
    Tree();

    unsigned int getHeight() const;

private:
    TreeNode root;
    unsigned int height; //TODO

friend class TreeIterator;
friend class TreeNode;

};

#endif // TREE_H
