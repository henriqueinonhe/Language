#include "parsingtreeconstiterator.h"

//NOTE Refactor using templates and helper classes!

ParsingTreeConstIterator::ParsingTreeConstIterator(const ParsingTree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

ParsingTreeConstIterator &ParsingTreeConstIterator::goToChild(const unsigned int index)
{
    if(index >= (uint) currentNode->children.size())
    {
        throw std::invalid_argument("This child does not exist.");
    }

    currentNode = currentNode->children[index].get();

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

    const QVector<unsigned int> pathVector = convertStringToPath(path);

    std::for_each(pathVector.begin(), pathVector.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });

    return *this;
}

ParsingTreeConstIterator &ParsingTreeConstIterator::travelPath(const QVector<unsigned int> &path)
{
    std::for_each(path.begin(), path.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });

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
    const QString uncencasedPath = removeOuterParenthesis(path);
    const QStringList coordinatesList = uncencasedPath.split(",");

    std::for_each(coordinatesList.begin(), coordinatesList.end(), [&](const QString &str)
    {
        pathVector.push_back(str.toInt());
    });

    return pathVector;
}

QString ParsingTreeConstIterator::removeOuterParenthesis(const QString &path) const
{
    const unsigned int parenthesisPadding = 1;
    const unsigned int numberOfParenthesis = 2;
    return path.mid(parenthesisPadding, path.size() - numberOfParenthesis * parenthesisPadding);
}
