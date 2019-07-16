#ifndef TYPEPARSINGTREEITERATOR_H
#define TYPEPARSINGTREEITERATOR_H

#include "typeparsingtree.h"

class TypeParsingTreeIterator
{
public:
    TypeParsingTreeIterator() = delete;
    TypeParsingTreeIterator(TypeParsingTree *tree);
    TypeParsingTreeIterator(const TypeParsingTree &) = delete;

    TypeParsingTreeIterator &operator =(const TypeParsingTreeIterator &) = delete;

    TypeParsingTreeIterator &goToChild(const unsigned int index);
    TypeParsingTreeIterator &goToParent();
    TypeParsingTreeIterator &goToRoot();
    TypeParsingTreeIterator &travelPath(const QString &path);
    TypeParsingTreeIterator &travelPath(const QVector<unsigned int> &coordinates);
    TypeParsingTreeIterator &goToCoordinates(const QString &coordinates);

    TypeParsingTree &getTree() const;

    TypeParsingTreeNode *operator->() const;
    TypeParsingTreeNode &operator*() const;

    ~TypeParsingTreeIterator() = default;

private:
    bool checkPathStringValidity(const QString &path) const;
    QVector<unsigned int> convertStringToPath(const QString &path) const;
    QString removeOuterParenthesis(const QString &path) const;

    TypeParsingTree *tree;
    TypeParsingTreeNode *currentNode;
};

#endif // TYPEPARSINGTREEITERATOR_H
