#ifndef TYPEPARSINGTREENODE_H
#define TYPEPARSINGTREENODE_H

#include <QString>
#include <QVector>
#include <memory>

class TypeTokenString;
class TypeParsingTreeIterator;
class TypeParsingTree;
class Type;

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
    unsigned int getGreatestDescendantHeight() const;

    MainOperator getMainOperator() const;
    void setMainOperator(const MainOperator &value);

    unsigned int getTypeBeginIndex() const;
    unsigned int getTypeEndIndex() const;

private:
    TypeParsingTreeNode(const TypeParsingTree *tree,
                        const TypeParsingTreeNode *parent,
                        const unsigned int typeBeginIndex,
                        const unsigned int typeEndIndex,
                        const MainOperator mainOperator = MainOperator::Primitive);

    void printNodeToString(QString &str) const;
    QString mainOperatorToString() const;

    const TypeParsingTree *tree;
    const TypeParsingTreeNode *parent;
    QVector<shared_ptr<TypeParsingTreeNode>> children;
    unsigned int typeBeginIndex;
    unsigned int typeEndIndex;
    MainOperator mainOperator;

friend class TypeParsingTreeIterator;
friend class TypeParsingTree;

};

#endif // TYPEPARSINGTREENODE_H
