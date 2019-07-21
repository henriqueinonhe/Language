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

class TypeParsingTreeNode final
{
public:
    enum class MainOperator : unsigned int
    {
        Primitive,
        Composition,
        Product
    };

    static MainOperator deserializeMainOperator(QDataStream &stream);

    TypeParsingTreeNode() = delete;
    TypeParsingTreeNode(const TypeParsingTreeNode &other) = delete;
    TypeParsingTreeNode(TypeParsingTreeNode &&other) noexcept = default;
    TypeParsingTreeNode &operator=(const TypeParsingTreeNode &other) = delete;
    TypeParsingTreeNode &operator=(TypeParsingTreeNode &&) = delete;
    ~TypeParsingTreeNode() = default;

    void appendChild(const unsigned int typeBeginIndex, const unsigned int typeEndIndex); //TODO Refactor this out

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

    TypeParsingTreeNode(const TypeParsingTreeNode &other,
                        TypeParsingTree *tree,
                        TypeParsingTreeNode *parent); //How could we make this private?
    TypeParsingTreeNode(QDataStream &stream,
                        TypeParsingTree *tree,
                        TypeParsingTreeNode *parent); //How could we make this private?

private:
    TypeParsingTreeNode(TypeParsingTree *tree,
                        TypeParsingTreeNode *parent,
                        const unsigned int typeBeginIndex,
                        const unsigned int typeEndIndex,
                        const MainOperator mainOperator = MainOperator::Primitive);


    void printNodeToString(QString &str) const;
    QString mainOperatorToString() const;
    void appendChild();
    QVector<shared_ptr<TypeParsingTreeNode>> deepCopyChildren(const QVector<shared_ptr<TypeParsingTreeNode>> &source,
                                                              TypeParsingTree *tree,
                                                              TypeParsingTreeNode *parent) const;

    TypeParsingTree * tree;
    TypeParsingTreeNode * parent;
    unsigned int typeBeginIndex;
    unsigned int typeEndIndex;
    MainOperator mainOperator;
    QVector<shared_ptr<TypeParsingTreeNode>> children;

friend class TypeParsingTreeConstIterator;
friend class TypeParsingTreeIterator;
friend class TypeParsingTree;
friend QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode &node);
friend QDataStream &operator >>(QDataStream &stream, TypeParsingTreeNode &node);
};

QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode &node);
QDataStream &operator <<(QDataStream &stream, const TypeParsingTreeNode::MainOperator mainOperator);
QDataStream &operator >>(QDataStream &stream, TypeParsingTreeNode::MainOperator &mainOperator);

#endif // TYPEPARSINGTREENODE_H
