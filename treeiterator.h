#ifndef TREEITERATOR_H
#define TREEITERATOR_H

#include "tree.h"
#include <QRegularExpression>

class TreeIterator
{
public:
    TreeIterator(Tree *tree);

    void goToChild(unsigned int index);
    void goToParent();
    void goToRoot();
    void travelPath(const QString &path);
    void goToCoordinates(const QString &coordinates);

    Tree getTree() const;

    TreeNode *operator->();

private:
    bool checkPathStringValidity(const QString &path);
    void convertPathToCoordinates(const QString &path, QVector<unsigned int> &vec);

    Tree *tree;
    TreeNode *currentNode;
};


#endif // TREEITERATOR_H
