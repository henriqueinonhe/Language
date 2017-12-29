#ifndef TYPEPARSINGTREEITERATOR_H
#define TYPEPARSINGTREEITERATOR_H

#include "TypeParsingTree.h"
#include <QRegularExpression>

class TypeParsingTreeIterator
{
public:
    TypeParsingTreeIterator(TypeParsingTree *tree);

    void goToChild(unsigned int index);
    void goToParent();
    void goToRoot();
    void travelPath(const QString &path);
    void goToCoordinates(const QString &coordinates);

    TypeParsingTree getTree() const;

    TypeParsingTreeNode *operator->();

private:
    bool checkPathStringValidity(const QString &path) const;
    void convertPathToCoordinates(const QString &path, QVector<unsigned int> &vec) const;

    TypeParsingTree *tree;
    TypeParsingTreeNode *currentNode;
};

#endif // TYPEPARSINGTREEITERATOR_H
