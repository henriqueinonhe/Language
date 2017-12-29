#ifndef TYPEPARSINGTREENODE_H
#define TYPEPARSINGTREENODE_H

#include <QString>
#include <QVector>
#include <memory>
#include "typetokenstring.h"

class TypeParsingTree;

using namespace std;

class TypeParsingTreeNode
{
public:
    void appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex);

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;

    TypeTokenString getTypeString() const;

    unsigned int getHeight() const;

private:
    TypeParsingTreeNode(TypeParsingTree *tree,
                        TypeParsingTreeNode *parent,
                        const QVector<unsigned int> &coordinates,
                        const unsigned int typeBeginIndex,
                        const unsigned int typeEndIndex);

    void printChildren(QString &str, const bool isLastChild);
    void printLastChild(QString &str);

    void updateTreeHeight();

    TypeParsingTree *tree;
    TypeParsingTreeNode *parent;
    QVector<unsigned int> coordinates;
    QVector<shared_ptr<TypeParsingTreeNode>> children;
    unsigned int typeBeginIndex;
    unsigned int typeEndIndex;

friend class TypeParsingTreeIterator;
friend class TypeParsingTree;

};

#endif // TYPEPARSINGTREENODE_H
