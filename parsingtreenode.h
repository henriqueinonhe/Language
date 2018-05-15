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
    void appendChild(const unsigned int BeginIndex, const unsigned int EndIndex);

    QVector<unsigned int> getCoordinates() const;
    QString coordinatesToString() const;

    bool isRoot() const;
    bool isChildless() const;

    TokenString getString() const;

    unsigned int getHeight() const;
    unsigned int getChildrenNumber() const;
    unsigned int getOwnChildNumber() const;

    unsigned int getBeginIndex() const;
    unsigned int getEndIndex() const;

private:
    ParsingTreeNode(ParsingTree *tree,
                        ParsingTreeNode *parent,
                        const QVector<unsigned int> &coordinates,
                        const unsigned int BeginIndex,
                        const unsigned int EndIndex);

    //void printNodeToString(QString &str) const; FIXME

    void updateTreeHeight();

    ParsingTree *tree;
    ParsingTreeNode *parent;
    QVector<unsigned int> coordinates;
    QVector<shared_ptr<ParsingTreeNode>> children;
    unsigned int BeginIndex;
    unsigned int EndIndex;

friend class ParsingTreeIterator;
friend class ParsingTree;

};

#endif // PARSINGTREENODE_H
