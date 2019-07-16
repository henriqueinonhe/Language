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
        for(auto node : nextLevelNodes)
        {
            node->printNodeToString(str);

            for(const auto &childNode : node->children)
            {
                nextLevelNodes2.push_back(childNode.get());
            }
        }

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
    //NOTE CHECK THIS! (Test for already initialized trees)
    const auto &copyNodeChildren = copyNode.children;
    auto &pasteNodeChildren = pasteNode.children;

    for(auto index = 0; index < copyNodeChildren.size(); index++)
    {
        pasteNodeChildren.push_back(shared_ptr<ParsingTreeNode>(new ParsingTreeNode(pasteNode.tree, &pasteNode)));
        pasteNodeChildren[index]->copyValues(*copyNodeChildren[index]);
        copyChildrenRecursively(*copyNodeChildren[index], *pasteNodeChildren[index]);
    }
}
