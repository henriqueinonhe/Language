#ifndef PARSINGTREENODE_H
#define PARSINGTREENODE_H

#include <QString>
#include <QVector>
#include <memory>
#include "tokenstring.h"

class ParsingTreeIterator;
class ParsingTree;

using namespace std;

class ParsingTreeNode
{
public:
    void appendChild(const unsigned int beginIndex, const unsigned int endIndex);

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;
    bool isChildless() const;

    TokenString getTokenString() const;

    unsigned int getHeight() const;
    unsigned int getChildrenNumber() const;
    unsigned int getOwnChildNumber() const;

    unsigned int getBeginIndex() const;
    unsigned int getEndIndex() const;

private:
    ParsingTreeNode(ParsingTree *tree,
                    ParsingTreeNode *parent,
                    const QVector<unsigned int> &coordinates,
                    const unsigned int beginIndex,
                    const unsigned int endIndex);

    void printNodeToString(QString &str) const;

    void updateTreeHeight();

    ParsingTree *tree;
    ParsingTreeNode *parent;
    QVector<unsigned int> coordinates;
    QVector<shared_ptr<ParsingTreeNode>> children;
    unsigned int beginIndex;
    unsigned int endIndex;

friend class ParsingTreeIterator;
friend class ParsingTree;

};

#endif // PARSINGTREENODE_H
