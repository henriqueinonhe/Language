#ifndef TYPEPARSINGTREE_H
#define TYPEPARSINGTREE_H

#include <QString>
#include <memory>
#include "typeparsingtreenode.h"
#include "typetoken.h"

using namespace std;

class TypeParsingTree
{
public:
    TypeParsingTree();
    TypeParsingTree(const QVector<TypeToken> &type);

    unsigned int getHeight() const;

private:
    TypeParsingTreeNode root;
    unsigned int height;
    QVector<TypeToken> type;

friend class TypeParsingTreeIterator;
friend class TypeParsingTreeNode;

};

#endif // TYPEPARSINGTREE_H
