#ifndef PARSINGTREECONSTITERATOR_H
#define PARSINGTREECONSTITERATOR_H

template <class T> class QVector;
class ParsingTree;
class ParsingTreeNode;
class QString;

class ParsingTreeConstIterator
{
public:
    ParsingTreeConstIterator(const ParsingTree *tree);

    ParsingTreeConstIterator &goToChild(const unsigned int index);
    ParsingTreeConstIterator &goToParent();
    ParsingTreeConstIterator &goToRoot();
    ParsingTreeConstIterator &travelPath(const QString &path);
    ParsingTreeConstIterator &travelPath(const QVector<unsigned int> &coordinates);
    ParsingTreeConstIterator &goToCoordinates(const QString &coordinates);

    const ParsingTree &getTree() const;

    const ParsingTreeNode *operator->();
    const ParsingTreeNode &operator*();

private:
    bool checkPathStringValidity(const QString &path) const;
    QVector<unsigned int> convertStringToPath(const QString &path) const;
    QString removeOuterParenthesis(const QString &path) const;

    const ParsingTree *tree;
    const ParsingTreeNode *currentNode;
};

#endif // PARSINGTREECONSTITERATOR_H
