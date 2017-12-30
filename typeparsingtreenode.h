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
    bool isChildless() const;

    TypeTokenString getTypeString() const;

    unsigned int getHeight() const;
    unsigned int getChildrenNumber() const;
    unsigned int getOwnChildNumber() const;


private:
    TypeParsingTreeNode(TypeParsingTree *tree,
                        TypeParsingTreeNode *parent,
                        const QVector<unsigned int> &coordinates,
                        const unsigned int typeBeginIndex,
                        const unsigned int typeEndIndex);

    void printNodeToString(QString &str);

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
