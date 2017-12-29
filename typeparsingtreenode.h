#ifndef TYPEPARSINGTREENODE_H
#define TYPEPARSINGTREENODE_H

#include <QVector>
#include <memory>
#include "typetokenstring.h"

class TypeParsingTree;

using namespace std;

class TypeParsingTreeNode
{
public:
    void appendChild();

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;

    TypeTokenString getTypeString() const;

    unsigned int getHeight() const;

private:
    TypeParsingTreeNode(TypeParsingTree *tree, TypeParsingTreeNode *parent, const QVector<unsigned int> &coordinates);

    void updateTreeHeight();

    TypeParsingTree *tree;
    TypeParsingTreeNode *parent;
    QVector<unsigned int> coordinates;
    QVector<shared_ptr<TypeParsingTreeNode>> children;
    unsigned int typeBeginIndex;
    unsigned int typeEndIndex;

friend class TypeParsingTreeIterator;
friend class TypeParsingTree;

};

#endif // TYPEPARSINGTREENODE_H
