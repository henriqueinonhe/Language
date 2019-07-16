#ifndef PARSINGTREEITERATOR_H
#define PARSINGTREEITERATOR_H

class ParsingTree;
class ParsingTreeNode;
class QString;
template <class T> class QVector;

class ParsingTreeIterator
{
public:
    ParsingTreeIterator() = delete;
    ParsingTreeIterator(ParsingTree *tree);
    ParsingTreeIterator(const ParsingTreeIterator &) = default;

    ParsingTreeIterator &operator=(const ParsingTreeIterator &) = delete;

    ParsingTreeIterator &goToChild(const unsigned int index);
    ParsingTreeIterator &goToParent();
    ParsingTreeIterator &goToRoot();
    ParsingTreeIterator &travelPath(const QString &path);
    ParsingTreeIterator &travelPath(const QVector<unsigned int> &coordinates);
    ParsingTreeIterator &goToCoordinates(const QString &coordinates);

    ParsingTree &getTree() const;

    ParsingTreeNode *operator->();
    ParsingTreeNode &operator*();

    ~ParsingTreeIterator() = default;

private:
    bool checkPathStringValidity(const QString &path) const;
    QVector<unsigned int> convertStringToPath(const QString &path) const;
    QString removeOuterParenthesis(const QString &path) const;

    ParsingTree *tree;
    ParsingTreeNode *currentNode;
};
#endif // PARSINGTREEITERATOR_H
