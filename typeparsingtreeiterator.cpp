#include "typeparsingtreeiterator.h"

TypeParsingTreeIterator::TypeParsingTreeIterator(TypeParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

void TypeParsingTreeIterator::goToChild(unsigned int index)
{
    if(index >= (uint) currentNode->children.size())
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

    const QVector<unsigned int> pathVector = convertStringToPath(path);

    std::for_each(pathVector.begin(), pathVector.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });
}

void TypeParsingTreeIterator::travelPath(const QVector<unsigned int> &path)
{
    std::for_each(path.begin(), path.end(), [&](unsigned int index)
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
    /* Path coordinates strings are in the form:
     * "(x1,x2,...,xn)". */

    QRegularExpression regex("^\\((\\d,)*\\d\\)$");

    return regex.match(path).hasMatch();
}

QVector<unsigned int> TypeParsingTreeIterator::convertStringToPath(const QString &path) const
{
    QVector<unsigned int> pathVector;
    const QString uncencasedPath = removeOuterParenthesis(path);
    const QStringList coordinatesList = uncencasedPath.split(",");

    std::for_each(coordinatesList.begin(), coordinatesList.end(), [&](const QString &str)
    {
        pathVector.push_back(str.toInt());
    });

    return pathVector;
}

QString TypeParsingTreeIterator::removeOuterParenthesis(const QString &path) const
{
    return path.mid(1, path.size() - 2);
}
