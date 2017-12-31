#include "typeparsingtreeiterator.h"

TypeParsingTreeIterator::TypeParsingTreeIterator(TypeParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

void TypeParsingTreeIterator::goToChild(unsigned int index)
{
    if(index >= currentNode->children.size())
    {
        throw std::invalid_argument("This child does not exist.");
    }

    currentNode = currentNode->children[index].get();
}

void TypeParsingTreeIterator::goToParent()
{
    currentNode = currentNode->parent;
}

void TypeParsingTreeIterator::goToRoot()
{
    currentNode = &tree->root;
}

void TypeParsingTreeIterator::travelPath(const QString &path)
{
    if(!checkPathStringValidity(path))
    {
        throw std::invalid_argument("This is not a valid path!");
    }

    QVector<unsigned int> vec;
    convertPathToCoordinates(path, vec);

    std::for_each(vec.begin(), vec.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });
}

void TypeParsingTreeIterator::travelPath(const QVector<unsigned int> &coordinates)
{
    std::for_each(coordinates.begin(), coordinates.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });
}

void TypeParsingTreeIterator::goToCoordinates(const QString &coordinates)
{
    if(!checkPathStringValidity(coordinates))
    {
        throw new std::invalid_argument("This is not a valid path!");
    }

    goToRoot();

    travelPath(coordinates);
}

TypeParsingTree &TypeParsingTreeIterator::getTree() const
{
    return *tree;
}

TypeParsingTreeNode *TypeParsingTreeIterator::operator->()
{
    return currentNode;
}

TypeParsingTreeNode &TypeParsingTreeIterator::operator*()
{
    return *currentNode;
}

bool TypeParsingTreeIterator::checkPathStringValidity(const QString &path) const
{
    QRegularExpression regex("^\\((\\d,)*\\d\\)$");

    return regex.match(path).hasMatch();
}

void TypeParsingTreeIterator::convertPathToCoordinates(const QString &path, QVector<unsigned int> &vec) const
{
    QString formattedPath = path.mid(1, path.size() - 2);
    QStringList list = formattedPath.split(",");

    std::for_each(list.begin(), list.end(), [&](const QString &str)
    {
        vec.push_back(str.toInt());
    });

}
