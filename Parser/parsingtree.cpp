#include "parsingtree.h"
#include "parsingtreeiterator.h"
#include "parsingtreeconstiterator.h"

ParsingTree::ParsingTree(const TokenString &string) :
    root(this, nullptr, 0, string.size() - 1),
    tokenString(string)
{
}

ParsingTree::ParsingTree(const ParsingTree &other) :
    root(this, nullptr)
{
    (*this) = other;

}

ParsingTree &ParsingTree::operator=(const ParsingTree &other)
{
    ParsingTreeConstIterator iter(&other);
    //Currently pointing at root!

    tokenString = other.getTokenString();
    root.copyValues(*iter);

    copyChildrenRecursively(*iter, root);

    return *this;
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

TokenString ParsingTree::getTokenString() const
{
    return tokenString;
}

void ParsingTree::copyChildrenRecursively(const ParsingTreeNode &copyNode, ParsingTreeNode &pasteNode) const
{
    const QVector<shared_ptr<ParsingTreeNode>> &copyNodeChildren = copyNode.children;
    QVector<shared_ptr<ParsingTreeNode>> &pasteNodeChildren = pasteNode.children;

    for(int index = 0; index < copyNodeChildren.size(); index++)
    {
        pasteNodeChildren.push_back(shared_ptr<ParsingTreeNode>(new ParsingTreeNode(pasteNode.tree, &pasteNode)));
        pasteNodeChildren[index]->copyValues(*copyNodeChildren[index]);
        copyChildrenRecursively(*copyNodeChildren[index], *pasteNodeChildren[index]);
    }
}
