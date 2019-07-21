#ifndef TYPEPARSINGTREE_H
#define TYPEPARSINGTREE_H

#include <memory>
#include "typeparsingtreenode.h"
#include "typetokenstring.h"

class TypeParsingTreeConstIterator;

using namespace std;

class TypeParsingTree final
{
public:
    TypeParsingTree() = delete;
    TypeParsingTree(const TypeParsingTree &other);
    TypeParsingTree(TypeParsingTree &&other) noexcept;
    TypeParsingTree &operator=(const TypeParsingTree &other) = delete;
    TypeParsingTree &operator=(TypeParsingTree &&) = delete;
    ~TypeParsingTree() = default;

    TypeParsingTree(QDataStream &stream);
    TypeParsingTree(const TypeTokenString &typeString);
    TypeParsingTree(TypeTokenString &&typeString);

    TypeParsingTreeIterator getIter();
    TypeParsingTreeConstIterator getConstIter() const;

    unsigned int getHeight() const;

    QString print();

    bool operator==(const TypeParsingTree &other) const;
    bool operator!=(const TypeParsingTree &other) const;
    bool deepEqualityCheck(const TypeParsingTree &other) const;

    TypeTokenString getTypeString() const;

private:
    TypeParsingTreeNode root;
    TypeTokenString typeString;

friend class TypeParsingTreeIterator;
friend class TypeParsingTreeNode;
friend class TypeParsingTreeConstIterator;
friend QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree);
};


QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree);

#endif // TYPEPARSINGTREE_H
