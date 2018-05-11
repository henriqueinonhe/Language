#include "treeiterator.h"

TreeIterator::TreeIterator(Tree *tree) :
    tree(tree),
    currentNode(&tree->root)
{

}

void TreeIterator::goToChild(unsigned int index)
{
    currentNode = currentNode->children[index].get();
}

void TreeIterator::goToParent()
{
    currentNode = currentNode->parent;
}

void TreeIterator::goToRoot()
{
    currentNode = &tree->root;
}

void TreeIterator::travelPath(const QString &path)
{
    if(!checkPathStringValidity(path))
    {
        throw std::invalid_argument("This is not a valid path!");
    }

    QVector<unsigned int> vec = convertPathToCoordinates(path);

    std::for_each(vec.begin(), vec.end(), [&](unsigned int index)
    {
        this->goToChild(index);
    });
}

void TreeIterator::goToCoordinates(const QString &coordinates)
{
    if(!checkPathStringValidity(coordinates))
    {
        throw new std::invalid_argument("This is not a valid path!");
    }

    goToRoot();

    travelPath(coordinates);
}

Tree TreeIterator::getTree() const
{
    return *tree;
}

TreeNode *TreeIterator::operator->()
{
    return currentNode;
}

bool TreeIterator::checkPathStringValidity(const QString &path) const
{
    /* Path coordinates strings have the following form:
     * "(x1,x2,...,xn)". */

    QRegularExpression regex("^\\((\\d,)*\\d\\)$");

    return regex.match(path).hasMatch();
}

QVector<unsigned int> TreeIterator::convertPathToCoordinates(const QString &path) const
{
    QVector<unsigned int> vec;
    const QString unencasedPath = removeOuterParenthesis(path);
    const QStringList coordinatesList = unencasedPath.split(",");

    std::for_each(coordinatesList.begin(), coordinatesList.end(), [&](const QString &str)
    {
        vec.push_back(str.toInt());
    });

    return vec;
}

QString TreeIterator::removeOuterParenthesis(const QString &path) const
{
    return path.mid(1, path.size() - 2);
}




