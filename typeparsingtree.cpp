#include "typeparsingtree.h"

TypeParsingTree::TypeParsingTree(const TypeTokenString &type) :
    root(this, nullptr, QVector<unsigned int>()),
    height(0),
    typeString(type)
{
    root.typeBeginIndex = 0;
    root.typeEndIndex = type.size() - 1;
}

unsigned int TypeParsingTree::getHeight() const
{
    return height;
}
