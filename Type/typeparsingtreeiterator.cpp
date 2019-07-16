#include "typeparsingtreeiterator.h"
#include <QRegularExpression>

TypeParsingTreeIterator::TypeParsingTreeIterator(TypeParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

TypeParsingTreeIterator &TypeParsingTreeIterator::goToChild(const unsigned int index)
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

    const auto pathVector = convertStringToPath(path);
    for(const auto index : pathVector)
    {
        this->goToChild(index);
    }

    return *this;
}

TypeParsingTreeIterator &TypeParsingTreeIterator::travelPath(const QVector<unsigned int> &path)
{
    for(const auto index : path)
    {
        this->goToChild(index);
    }

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
    const auto uncencasedPath = removeOuterParenthesis(path);
    const auto coordinatesStringList = uncencasedPath.split(",");
    for(const auto str : coordinatesStringList)
    {
        pathVector.push_back(static_cast<unsigned int>(str.toInt()));
    }

    return pathVector;
}

QString TypeParsingTreeIterator::removeOuterParenthesis(const QString &path) const
{
    const auto parenthesisPadding = 1;
    const auto numberOfParenthesis = 2;
    return path.mid(parenthesisPadding, path.size() - static_cast<int>(numberOfParenthesis * parenthesisPadding));
}
