#include "parsingtreeconstiterator.h"
#include <QRegularExpression>
#include "parsingtreenode.h"
#include "parsingtree.h"

//NOTE Refactor using templates and helper classes!

ParsingTreeConstIterator::ParsingTreeConstIterator(const ParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

ParsingTreeConstIterator &ParsingTreeConstIterator::goToChild(const unsigned int index)
{
    if(index >= static_cast<uint>(currentNode->children.size()))
    {
        throw std::invalid_argument("This child does not exist.");
    }

    currentNode = currentNode->children[static_cast<int>(index)].get();

    return *this;
}

ParsingTreeConstIterator &ParsingTreeConstIterator::goToParent()
{
    currentNode = const_cast<ParsingTreeNode *>(currentNode->parent);

    return *this;
}

ParsingTreeConstIterator &ParsingTreeConstIterator::goToRoot()
{
    currentNode = &tree->root;

    return *this;
}

ParsingTreeConstIterator &ParsingTreeConstIterator::travelPath(const QString &path)
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

ParsingTreeConstIterator &ParsingTreeConstIterator::travelPath(const QVector<unsigned int> &path)
{
    for(auto index : path)
    {
        this->goToChild(index);
    }

    return *this;
}

ParsingTreeConstIterator &ParsingTreeConstIterator::goToCoordinates(const QString &coordinates)
{
    if(!checkPathStringValidity(coordinates))
    {
        throw new std::invalid_argument("This is not a valid path!");
    }

    goToRoot();

    travelPath(coordinates);

    return *this;
}

const ParsingTree &ParsingTreeConstIterator::getTree() const
{
    return *tree;
}

const ParsingTreeNode *ParsingTreeConstIterator::operator->()
{
    return currentNode;
}

const ParsingTreeNode &ParsingTreeConstIterator::operator*()
{
    return *currentNode;
}

bool ParsingTreeConstIterator::checkPathStringValidity(const QString &path) const
{
    /* Path coordinates strings are in the form:
     * "(x1,x2,...,xn)". */

    QRegularExpression regex("^\\((\\d,)*\\d\\)$");

    return regex.match(path).hasMatch();
}

QVector<unsigned int> ParsingTreeConstIterator::convertStringToPath(const QString &path) const
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

QString ParsingTreeConstIterator::removeOuterParenthesis(const QString &path) const
{
    const auto parenthesisPadding = 1;
    const auto numberOfParenthesis = 2;
    return path.mid(parenthesisPadding, static_cast<int>(static_cast<uint>(path.size()) - numberOfParenthesis * parenthesisPadding));
}
