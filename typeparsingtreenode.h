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
    enum class MainOperator
    {
        Primitive,
        Composition,
        Product
    };

    void appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex);

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;
    bool isChildless() const;

    TypeTokenString getTypeString() const;

    unsigned int getHeight() const;
    unsigned int getChildrenNumber() const;
    unsigned int getOwnChildNumber() const;


    MainOperator getMainOperator() const;
    void setMainOperator(const MainOperator &value);

private:
    TypeParsingTreeNode(TypeParsingTree *tree,
                        TypeParsingTreeNode *parent,
                        const QVector<unsigned int> &coordinates,
                        const unsigned int typeBeginIndex,
                        const unsigned int typeEndIndex,
                        const MainOperator mainOperator = MainOperator::Primitive);

    void printNodeToString(QString &str) const;
    QString mainOperatorToString() const;

    void updateTreeHeight();

    TypeParsingTree *tree;
    TypeParsingTreeNode *parent;
    QVector<unsigned int> coordinates;
    QVector<shared_ptr<TypeParsingTreeNode>> children;
    unsigned int typeBeginIndex;
    unsigned int typeEndIndex;
    MainOperator mainOperator;

friend class TypeParsingTreeIterator;
friend class TypeParsingTree;

};

#endif // TYPEPARSINGTREENODE_H
