#include "typeparsingtreeiterator.h"
#include <QRegularExpression>

TypeParsingTreeIterator::TypeParsingTreeIterator(TypeParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

TypeParsingTreeIterator &TypeParsingTreeIterator::goToChild(unsigned int index)
{
    if(index >= static_cast<unsigned int>( currentNode->children.size()))
    {
        throw std::invalid_argument("This child does not exist.");
    }

    currentNode = currentNode->children[static_cast<int>(index)].get();

    return *this;
}

TypeParsingTreeIterator &TypeParsingTreeIterator::goToParent()
{
    currentNode = const_cast<TypeParsingTreeNode *>(currentNode->parent);

    return *this;
}

TypeParsingTreeIterator &TypeParsingTreeIterator::goToRoot()
{
    currentNode = &tree->root;

    return *this;
}

TypeParsingTreeIterator &TypeParsingTreeIterator::travelPath(const QString &path)
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

    return *this;
}

TypeParsingTreeIterator &TypeParsingTreeIterator::travelPath(const QVector<unsigned int> &path)
{
    std::for_each(path.begin(), path.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });

    return *this;
}

TypeParsingTreeIterator &TypeParsingTreeIterator::goToCoordinates(const QString &coordinates)
{
    if(!checkPathStringValidity(coordinates))
    {
        throw new std::invalid_argument("This is not a valid path!");
    }

    goToRoot();

    travelPath(coordinates);

    return *this;
}

TypeParsingTree &TypeParsingTreeIterator::getTree() const
{
    return *tree;
}

TypeParsingTreeNode *TypeParsingTreeIterator::operator->() const
{
    return currentNode;
}

TypeParsingTreeNode &TypeParsingTreeIterator::operator*() const
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
    /* Path coordinates strings are in the form:
     * "(x1,x2,...,xn)". */

    QVector<unsigned int> pathVector;
    const QString uncencasedPath = removeOuterParenthesis(path);
    const QStringList coordinatesList = uncencasedPath.split(",");

    std::for_each(coordinatesList.begin(), coordinatesList.end(), [&](const QString &str)
    {
        pathVector.push_back(static_cast<unsigned int>(str.toInt()));
    });

    return pathVector;
}

QString TypeParsingTreeIterator::removeOuterParenthesis(const QString &path) const
{
    const unsigned int parenthesisPadding = 1;
    const unsigned int numberOfParenthesis = 2;
    return path.mid(parenthesisPadding, path.size() - static_cast<int>(numberOfParenthesis * parenthesisPadding));
}
