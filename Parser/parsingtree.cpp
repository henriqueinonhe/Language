#include "parsingtree.h"
#include "parsingtreeiterator.h"

ParsingTree::ParsingTree(const TokenString &string) :
    root(this, nullptr, 0, string.size() - 1),
    tokenString(string)
{
}

unsigned int ParsingTree::getHeight() const
{
    return root.getGreatestDescendantHeight();
}

QString ParsingTree::print()
{
    ParsingTreeIterator iter(this);
    QVector<ParsingTreeNode *> nextLevelNodes{&(*iter)};
    QString str;

    for(int currentLevel = 0; currentLevel <= static_cast<int>(this->getHeight()); currentLevel++)
    {
        QVector<ParsingTreeNode *> nextLevelNodes2;
        std::for_each(nextLevelNodes.begin(), nextLevelNodes.end(), [&str, &nextLevelNodes2](ParsingTreeNode *node)
        {

            node->printNodeToString(str);

            std::for_each(node->children.begin(), node->children.end(), [&nextLevelNodes2](const shared_ptr<ParsingTreeNode> &node)
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

bool ParsingTree::operator==(const ParsingTree &other) const
{
    return this->tokenString == other.tokenString;
}

bool ParsingTree::operator!=(const ParsingTree &other) const
{
    return !(*this == other);
}
