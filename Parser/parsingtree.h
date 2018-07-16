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
    ParsingTree(const TokenString &String);

    ParsingTree(const ParsingTree &other) = delete;
    ParsingTree &operator=(const ParsingTree &other) = delete;

    unsigned int getHeight() const;

    QString print();

    bool operator==(const ParsingTree &other) const;
    bool operator!=(const ParsingTree &other) const;

private:
    ParsingTreeNode root;
    unsigned int height;
    TokenString tokenString;

friend class ParsingTreeIterator;
friend class ParsingTreeNode;

};
#endif // PARSINGTREE_H
