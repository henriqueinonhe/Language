#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"

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
    /* This algorithm prints the tree textually for debugging/inspection purposes
     * and it does so in a level by level ordering.
     * It works mainly with two vectors, one holds the nodes currently
     * being printed and the other the children of the former, that is,
     * the nodes that will be printed next.
     * Lastly we swap vectors when the first row of nodes has been already printed,
     * for as we already have the next row held by the second vector, we don't need
     * the first vector anymore, therefore we can copy all of the elements of the
     * second vector to the first, saving memory. */

    TypeParsingTreeIterator iter(this);
    QVector<TypeParsingTreeNode *> nextLevelNodes{&(*iter)};
    QString str;

    for(int currentLevel = 0; currentLevel <= (int) this->getHeight(); currentLevel++)
    {
        QVector<TypeParsingTreeNode *> nextLevelNodes2;
        std::for_each(nextLevelNodes.begin(),
                      nextLevelNodes.end(),
                      [&str, &nextLevelNodes2](TypeParsingTreeNode *node)
        {

            node->printNodeToString(str);

            std::for_each(node->children.begin(),
                          node->children.end(),
                          [&nextLevelNodes2](const shared_ptr<TypeParsingTreeNode> &node)
            {
                nextLevelNodes2.push_back(node.get());
            });
        });
        str += "\n\n";
        nextLevelNodes.swap(nextLevelNodes2);
    }

    str += "------------------------------\n";

    return str;
}

bool TypeParsingTree::operator==(const TypeParsingTree &other) const
{
    return this->typeString == other.typeString;
}

bool TypeParsingTree::operator!=(const TypeParsingTree &other) const
{
    return !(*this == other);
}

TypeTokenString TypeParsingTree::getTypeString() const
{
    return typeString;
}
