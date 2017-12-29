#include "typeparsingtree.h"

TypeParsingTree::TypeParsingTree(const TypeTokenString &type) :
    root(this, nullptr, QVector<unsigned int>(), 0, type.size() - 1),
    height(0),
    typeString(type)
{
}

unsigned int TypeParsingTree::getHeight() const
{
    return height;
}

QString TypeParsingTree::print()
{
    QString str;

    str += "{";
    str += root.getTypeString().toString();
    str += "}\n\n";

    root.printChildren(str, true);

    return str;
}
