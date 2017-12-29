#include "typeparsingtree.h"

TypeParsingTree::TypeParsingTree() :
    root(this, nullptr, QVector<unsigned int>()),
    height(0)
{

}

TypeParsingTree::TypeParsingTree(const QVector<TypeToken> &type) :
    root(this, nullptr, QVector<unsigned int>()),
    height(0),
    type(type)
{
    root.typeBeginIndex = 0;
    root.typeEndIndex = type.size() - 1;
}

unsigned int TypeParsingTree::getHeight() const
{
    return height;
}
