#ifndef TYPEPARSINGTREE_H
#define TYPEPARSINGTREE_H

#include <memory>
#include "typeparsingtreenode.h"
#include "typetokenstring.h"

using namespace std;

class TypeParsingTree //NOTE Maybe I should implement e deep copy option here! Or a Const Iterator.
{
public:
    TypeParsingTree(QDataStream &stream);
    TypeParsingTree(const TypeTokenString &typeString);

    TypeParsingTree(const TypeParsingTree &other);
    TypeParsingTree &operator=(const TypeParsingTree &other);

    unsigned int getHeight() const;

    QString print();

    bool operator==(const TypeParsingTree &other) const;
    bool operator!=(const TypeParsingTree &other) const;

    TypeTokenString getTypeString() const;

private:
    TypeParsingTreeNode root;
    TypeTokenString typeString;

friend class TypeParsingTreeIterator;
friend class TypeParsingTreeNode;
friend QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree);
friend QDataStream &operator >>(QDataStream &stream, TypeParsingTree &tree);

};


QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree);
QDataStream &operator >>(QDataStream &stream, TypeParsingTree &tree);

#endif // TYPEPARSINGTREE_H
