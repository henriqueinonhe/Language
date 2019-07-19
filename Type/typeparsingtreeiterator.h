#ifndef TYPEPARSINGTREEITERATOR_H
#define TYPEPARSINGTREEITERATOR_H

#include "typeparsingtree.h"

class TypeParsingTreeIterator
{
public:
    TypeParsingTreeIterator() = delete;
    TypeParsingTreeIterator(const TypeParsingTreeIterator &) = default;
    TypeParsingTreeIterator(TypeParsingTreeIterator &&) = default;
    TypeParsingTreeIterator &operator =(const TypeParsingTreeIterator &) = default;
    TypeParsingTreeIterator &operator =(TypeParsingTreeIterator &&) = default;
    ~TypeParsingTreeIterator() = default;

    TypeParsingTreeIterator(TypeParsingTree *tree);

    TypeParsingTreeIterator &goToChild(const unsigned int index);
    TypeParsingTreeIterator &goToParent();
    TypeParsingTreeIterator &goToRoot();
    TypeParsingTreeIterator &travelPath(const QString &path);
    TypeParsingTreeIterator &travelPath(const QVector<unsigned int> &coordinates);
    TypeParsingTreeIterator &goToCoordinates(const QString &coordinates);

    TypeParsingTree &getTree() const;

    TypeParsingTreeNode *operator->() const;
    TypeParsingTreeNode &operator*() const;

private:
    bool checkPathStringValidity(const QString &path) const;
    QVector<unsigned int> convertStringToPath(const QString &path) const;
    QString removeOuterParenthesis(const QString &path) const;

    TypeParsingTree *tree;
    TypeParsingTreeNode *currentNode;
};

#endif // TYPEPARSINGTREEITERATOR_H
