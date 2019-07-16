#ifndef PARSINGTREE_H
#define PARSINGTREE_H

#include <memory>
#include "parsingtreenode.h"
#include "tokenstring.h"

class QString;

using namespace std;

class ParsingTree
{
public:
    ParsingTree() = delete;
    ParsingTree(const TokenString &string);

    ParsingTree(const ParsingTree &other);
    ParsingTree &operator=(const ParsingTree &other);

    unsigned int getHeight() const;

    QString print();

    bool operator==(const ParsingTree &other) const;
    bool operator!=(const ParsingTree &other) const;

    TokenString getTokenString() const;

    ~ParsingTree() = default;

private:
    void copyChildrenRecursively(const ParsingTreeNode &copyNode, ParsingTreeNode &pasteNode) const;

    ParsingTreeNode root;
    TokenString tokenString;

friend class ParsingTreeIterator;
friend class ParsingTreeConstIterator;
friend class ParsingTreeNode;

};


//Reminder to self: There is no sensible way to serialize this parsing tree without
//rebuilding it, mainly because Tokens are stored in a pool (signature) and therefore
//to deserealize a TokenString (and therefore whatever constructs that depend on it)
//we need to provide a signature as well, so given that right now performance isn't an issue
//I won't implement serialization on parsing trees

#endif // PARSINGTREE_H
