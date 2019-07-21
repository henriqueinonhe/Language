#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "typeparsingtreeconstiterator.h"

TypeParsingTree::TypeParsingTree(QDataStream &stream) :
    root(stream, this, nullptr),
    typeString(stream)
{

}

TypeParsingTree::TypeParsingTree(const TypeTokenString &type) :
    root(this, nullptr, 0, type.size() - 1),
    typeString(type)
{
}

TypeParsingTree::TypeParsingTree(TypeTokenString &&typeString) :
    root(this, nullptr, 0, typeString.size() - 1),
    typeString(std::move(typeString))
{

}

TypeParsingTree::TypeParsingTree(const TypeParsingTree &other) :
    root(other.root, this, nullptr),
    typeString(other.typeString)
{
}

TypeParsingTree::TypeParsingTree(TypeParsingTree &&other) noexcept :
    root(other.root, this, &root),
    typeString(std::move(other.typeString))
{

}

TypeParsingTreeIterator TypeParsingTree::getIter()
{
    return TypeParsingTreeIterator(this);
}

TypeParsingTreeConstIterator TypeParsingTree::getConstIter() const
{
    return TypeParsingTreeConstIterator(this);
}

unsigned int TypeParsingTree::getHeight() const
{
    return root.getGreatestDescendantHeight();
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

    auto iter = getIter();
    QVector<TypeParsingTreeNode *> nextLevelNodes{&(*iter)};
    QString str;

    for(auto currentLevel = 0; currentLevel <= static_cast<int>(this->getHeight()); currentLevel++)
    {
        QVector<TypeParsingTreeNode *> nextLevelNodes2;

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

bool TypeParsingTree::operator==(const TypeParsingTree &other) const
{
    return this->typeString == other.typeString;
}

bool TypeParsingTree::operator!=(const TypeParsingTree &other) const
{
    return !(*this == other);
}

bool TypeParsingTree::deepEqualityCheck(const TypeParsingTree &other) const
{
    //Not sure what is the purpose of this method
    //once the == operator should suffice (given the class invariants).
    if(this->typeString != other.typeString)
    {
        return false;
    }

    return this->root == other.root;
}

TypeTokenString TypeParsingTree::getTypeString() const
{
    return typeString;
}

QDataStream &operator <<(QDataStream &stream, const TypeParsingTree &tree)
{
    stream << tree.root << tree.typeString;
    return stream;
}
