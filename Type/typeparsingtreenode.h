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
    enum class MainOperator : unsigned int
    {
        Primitive,
        Composition,
        Product
    };

    TypeParsingTreeNode() = delete;
    TypeParsingTreeNode(const TypeParsingTreeNode &other) = delete;
    TypeParsingTreeNode &operator=(const TypeParsingTreeNode &other) = delete;

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

    bool operator==(const TypeParsingTreeNode &other) const;
    bool operator!=(const TypeParsingTreeNode &other) const;

private:
    TypeParsingTreeNode(QDataStream &stream);
    TypeParsingTreeNode(const TypeParsingTreeNode &other, const TypeParsingTree *tree);
    TypeParsingTreeNode(const TypeParsingTree *tree,
                        const TypeParsingTreeNode *parent,
                        const unsigned int typeBeginIndex,
                        const unsigned int typeEndIndex,
                        const MainOperator mainOperator = MainOperator::Primitive);


    void printNodeToString(QString &str) const;
    QString mainOperatorToString() const;
    void appendChild();

    void copyValues(const TypeParsingTreeNode &other);

    const TypeParsingTree *tree;
    const TypeParsingTreeNode *parent;
    QVector<shared_ptr<TypeParsingTreeNode>> children;
    unsigned int typeBeginIndex;
    unsigned int typeEndIndex;
    MainOperator mainOperator;

friend class TypeParsingTreeIterator;
friend class TypeParsingTree;
friend QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode &node);
friend QDataStream &operator >>(QDataStream &stream, TypeParsingTreeNode &node);

};

QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode &node);
QDataStream &operator >>(QDataStream &stream, TypeParsingTreeNode &node);
QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode::MainOperator mainOperator);
QDataStream &operator >>(QDataStream &stream, TypeParsingTreeNode::MainOperator &mainOperator);

#endif // TYPEPARSINGTREENODE_H
