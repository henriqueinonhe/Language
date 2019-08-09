#ifndef PARSINGTREENODE_H
#define PARSINGTREENODE_H

#include <QSet>
#include <memory>
#include "type.h"

class VariableToken;
class TokenString;
class ParsingTreeIterator;
class ParsingTree;
class Parser;

using namespace std;

class ParsingTreeNode
{
public:
    ParsingTreeNode() = delete;

    ParsingTreeNode &operator =(const ParsingTreeNode &) = delete;

    void appendChild(const unsigned int beginIndex, const unsigned int endIndex);

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;
    bool isChildless() const;

    TokenString getTokenString() const;

    unsigned int getHeight() const;
    unsigned int getChildrenNumber() const;
    unsigned int getOwnChildNumber() const;
    unsigned int getGreatestDescendantHeight() const;

    unsigned int getBeginIndex() const;
    unsigned int getEndIndex() const;

    bool hasSingleToken() const;

    Type getType() const;
    void setType(const Type &value);

    ~ParsingTreeNode() = default;

private:
    ParsingTreeNode(const ParsingTree *tree,
                    const ParsingTreeNode *parent);
    ParsingTreeNode(const ParsingTree *tree,
                    const ParsingTreeNode *parent,
                    const unsigned int beginIndex,
                    const unsigned int endIndex);

    void copyValues(const ParsingTreeNode &other);

    void printNodeToString(QString &str) const;

    QString printVariableSet(const QSet<const VariableToken *> &set) const;

    const ParsingTree *tree;
    const ParsingTreeNode *parent;
    QVector<shared_ptr<ParsingTreeNode>> children;
    unsigned int beginIndex;
    unsigned int endIndex;
    Type type;
    QSet<const VariableToken *> freeVariables;
    QSet<const VariableToken *> boundVariables;

    friend class ParsingTreeConstIterator;
    friend class ParsingTreeIterator;
    friend class ParsingTree;
    friend class Parser;
};

#endif // PARSINGTREENODE_H
