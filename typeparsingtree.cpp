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
    TypeParsingTreeIterator iter(this);
    QVector<TypeParsingTreeNode *> nextLevelNodes{&(*iter)};
    QString str;

    for(int currentLevel = 0; currentLevel <= (int) this->getHeight(); currentLevel++)
    {
        QVector<TypeParsingTreeNode *> nextLevelNodes2;
        std::for_each(nextLevelNodes.begin(),
                      nextLevelNodes.end(),
                      [&str, &nextLevelNodes2](TypeParsingTreeNode *node) {

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
