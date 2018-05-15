#ifndef PARSINGTREEITERATOR_H
#define PARSINGTREEITERATOR_H

#include "parsingtree.h"
#include <QRegularExpression>

class ParsingTreeIterator
{
public:
    ParsingTreeIterator(ParsingTree *tree);

    void goToChild(unsigned int index);
    void goToParent();
    void goToRoot();
    void travelPath(const QString &path);
    void travelPath(const QVector<unsigned int> &coordinates);
    void goToCoordinates(const QString &coordinates);

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
