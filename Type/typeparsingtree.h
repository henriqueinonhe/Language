#ifndef TYPEPARSINGTREE_H
#define TYPEPARSINGTREE_H

#include <memory>
#include "typeparsingtreenode.h"
#include "typetokenstring.h"

/* Class Invariant:
 * A TypeParsingTree holds the same const TypeTokenString
 * until the end of its life, therefore it is not copy constructible
   assignable. */

using namespace std;

class TypeParsingTree final //NOTE Maybe I should implement a Const Iterator.
{
public:
    TypeParsingTree() = delete;
    TypeParsingTree(const TypeParsingTree &other);
    TypeParsingTree(TypeParsingTree &&) = default;
    TypeParsingTree &operator=(const TypeParsingTree &other) = delete;
    TypeParsingTree &operator=(TypeParsingTree &&) = default;
    ~TypeParsingTree() = default;

    TypeParsingTree(QDataStream &stream);
    TypeParsingTree(const TypeTokenString &typeString);

    TypeParsingTreeIterator getIter();

    unsigned int getHeight() const;

    QString print();

    bool operator==(const TypeParsingTree &other) const;
    bool operator!=(const TypeParsingTree &other) const;
    bool deepEqualityCheck(const TypeParsingTree &other) const;

    const TypeTokenString &getTypeString() const;

private:
    TypeParsingTreeNode root;
    const TypeTokenString typeString;

friend class TypeParsingTreeIterator;
friend class TypeParsingTreeNode;
friend class TypeParsingTreeConstIterator;
friend QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree);
};


QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree);

#endif // TYPEPARSINGTREE_H
