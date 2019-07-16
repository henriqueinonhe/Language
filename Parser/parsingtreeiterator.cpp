#include "parsingtreeiterator.h"
#include "parsingtree.h"
#include <QRegularExpression>

ParsingTreeIterator::ParsingTreeIterator(ParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

ParsingTreeIterator &ParsingTreeIterator::goToChild(const unsigned int index)
{
    if(index >= static_cast<uint>(currentNode->children.size()))
    {
        throw std::invalid_argument("This child does not exist.");
    }

    currentNode = currentNode->children[static_cast<int>(index)].get();

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::goToParent()
{
    currentNode = const_cast<ParsingTreeNode *>(currentNode->parent);

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::goToRoot()
{
    currentNode = &tree->root;

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::travelPath(const QString &path)
{
    if(!checkPathStringValidity(path))
    {
        throw std::invalid_argument("This is not a valid path!");
    }

    const auto pathVector = convertStringToPath(path);
    for(auto index : pathVector)
    {
        this->goToChild(index);
    }

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::travelPath(const QVector<unsigned int> &path)
{
    for(auto index : path)
    {
        this->goToChild(index);
    }

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::goToCoordinates(const QString &coordinates)
{
    if(!checkPathStringValidity(coordinates))
    {
        throw new std::invalid_argument("This is not a valid path!");
    }

    goToRoot();

    travelPath(coordinates);

    return *this;
}

ParsingTree &ParsingTreeIterator::getTree() const
{
    return *tree;
}

ParsingTreeNode *ParsingTreeIterator::operator->()
{
    return currentNode;
}

ParsingTreeNode &ParsingTreeIterator::operator*()
{
    return *currentNode;
}

bool ParsingTreeIterator::checkPathStringValidity(const QString &path) const
{
    /* Path coordinates strings are in the form:
     * "(x1,x2,...,xn)". */

    QRegularExpression regex("^\\((\\d,)*\\d\\)$");

    return regex.match(path).hasMatch();
}

QVector<unsigned int> ParsingTreeIterator::convertStringToPath(const QString &path) const
{
    QVector<unsigned int> pathVector;
    const auto uncencasedPath = removeOuterParenthesis(path);
    const auto coordinatesList = uncencasedPath.split(",");

    for(const auto &str : coordinatesList)
    {
        pathVector.push_back(static_cast<uint>(str.toInt()));
    }

    return pathVector;
}

QString ParsingTreeIterator::removeOuterParenthesis(const QString &path) const
{
    const auto parenthesisPadding = 1;
    const auto numberOfParenthesis = 2;
    return path.mid(parenthesisPadding, static_cast<int>(static_cast<uint>(path.size()) - numberOfParenthesis * parenthesisPadding));
}
