#include "parsingtreeiterator.h"

ParsingTreeIterator::ParsingTreeIterator(ParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

ParsingTreeIterator &ParsingTreeIterator::goToChild(const unsigned int index)
{
    if(index >= (uint) currentNode->children.size())
    {
        throw std::invalid_argument("This child does not exist.");
    }

    currentNode = currentNode->children[index].get();

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::goToParent()
{
    currentNode = currentNode->parent;

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

    const QVector<unsigned int> pathVector = convertStringToPath(path);

    std::for_each(pathVector.begin(), pathVector.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });

    return *this;
}

ParsingTreeIterator &ParsingTreeIterator::travelPath(const QVector<unsigned int> &path)
{
    std::for_each(path.begin(), path.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });

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
    const QString uncencasedPath = removeOuterParenthesis(path);
    const QStringList coordinatesList = uncencasedPath.split(",");

    std::for_each(coordinatesList.begin(), coordinatesList.end(), [&](const QString &str)
    {
        pathVector.push_back(str.toInt());
    });

    return pathVector;
}

QString ParsingTreeIterator::removeOuterParenthesis(const QString &path) const
{
    const unsigned int parenthesisPadding = 1;
    const unsigned int numberOfParenthesis = 2;
    return path.mid(parenthesisPadding, path.size() - numberOfParenthesis * parenthesisPadding);
}
