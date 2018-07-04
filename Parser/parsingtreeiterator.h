#ifndef PARSINGTREEITERATOR_H
#define PARSINGTREEITERATOR_H

#include "parsingtree.h"
#include <QRegularExpression>

class ParsingTreeIterator
{
public:
    ParsingTreeIterator(ParsingTree *tree);

    ParsingTreeIterator &goToChild(unsigned int index);
    ParsingTreeIterator &goToParent();
    ParsingTreeIterator &goToRoot();
    ParsingTreeIterator &travelPath(const QString &path);
    ParsingTreeIterator &travelPath(const QVector<unsigned int> &coordinates);
    ParsingTreeIterator &goToCoordinates(const QString &coordinates);

    ParsingTree &getTree() const;

    ParsingTreeNode *operator->();
    ParsingTreeNode &operator*();

private:
    bool checkPathStringValidity(const QString &path) const;
    QVector<unsigned int> convertStringToPath(const QString &path) const;
    QString removeOuterParenthesis(const QString &path) const;

    ParsingTree *tree;
    ParsingTreeNode *currentNode;
};
#endif // PARSINGTREEITERATOR_H
