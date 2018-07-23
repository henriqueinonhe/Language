#ifndef PARSINGTREE_H
#define PARSINGTREE_H

#include <QString>
#include <memory>
#include "parsingtreenode.h"
#include "tokenstring.h"

using namespace std;

class ParsingTree
{
public:
    ParsingTree(const TokenString &string);

    ParsingTree(const ParsingTree &other);
    ParsingTree &operator=(const ParsingTree &other);

    unsigned int getHeight() const;

    QString print();

    bool operator==(const ParsingTree &other) const;
    bool operator!=(const ParsingTree &other) const;

    TokenString getTokenString() const;

private:
    void copyChildrenRecursively(const ParsingTreeNode &copyNode, ParsingTreeNode &pasteNode) const;

    ParsingTreeNode root;
    TokenString tokenString;

friend class ParsingTreeIterator;
friend class ParsingTreeConstIterator;
friend class ParsingTreeNode;

};


#endif // PARSINGTREE_H
