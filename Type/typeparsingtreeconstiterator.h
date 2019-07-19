#ifndef TYPEPARSINGTREECONSTITERATOR_H
#define TYPEPARSINGTREECONSTITERATOR_H

#include "typeparsingtree.h"

class TypeParsingTreeConstIterator
{
public:
    TypeParsingTreeConstIterator() = delete;
    TypeParsingTreeConstIterator(const TypeParsingTreeConstIterator &) = default;
    TypeParsingTreeConstIterator(TypeParsingTreeConstIterator &&) = default;
    TypeParsingTreeConstIterator &operator =(const TypeParsingTreeConstIterator &) = default;
    TypeParsingTreeConstIterator &operator =(TypeParsingTreeConstIterator &&) = default;
    ~TypeParsingTreeConstIterator() = default;

    TypeParsingTreeConstIterator(const TypeParsingTree *tree);

    TypeParsingTreeConstIterator &goToChild(const unsigned int index);
    TypeParsingTreeConstIterator &goToParent();
    TypeParsingTreeConstIterator &goToRoot();
    TypeParsingTreeConstIterator &travelPath(const QString &path);
    TypeParsingTreeConstIterator &travelPath(const QVector<unsigned int> &coordinates);
    TypeParsingTreeConstIterator &goToCoordinates(const QString &coordinates);

    const TypeParsingTree &getTree() const;

    const TypeParsingTreeNode *operator->() const;
    const TypeParsingTreeNode &operator*() const;

private:
    bool checkPathStringValidity(const QString &path) const;
    QVector<unsigned int> convertStringToPath(const QString &path) const;
    QString removeOuterParenthesis(const QString &path) const;

    const TypeParsingTree *tree;
    const TypeParsingTreeNode *currentNode;
};

#endif // TYPEPARSINGTREECONSTITERATOR_H
