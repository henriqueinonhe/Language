#ifndef TYPEPARSINGTREE_H
#define TYPEPARSINGTREE_H

#include <QString>
#include <memory>
#include "typeparsingtreenode.h"
#include "typetokenstring.h"
using namespace std;

class TypeParsingTree
{
public:


    TypeParsingTree(const TypeTokenString &typeString);

    TypeParsingTree(const TypeParsingTree &other) = delete;
    TypeParsingTree &operator=(const TypeParsingTree &other) = delete;

    unsigned int getHeight() const;

    QString print();

    bool operator==(const TypeParsingTree &other) const;
    bool operator!=(const TypeParsingTree &other) const;

private:
    TypeParsingTreeNode root;
    unsigned int height;
    TypeTokenString typeString;

friend class TypeParsingTreeIterator;
friend class TypeParsingTreeNode;

};

#endif // TYPEPARSINGTREE_H
