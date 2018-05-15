#ifndef PARSINGTREE_H
#define PARSINGTREE_H

#include <QString>

template <class T>

class ParsingTree
{
public:


    ParsingTree(const T &string)
    {

    }

    ParsingTree(const ParsingTree &other) = delete;
    ParsingTree &operator=(const ParsingTree &other) = delete;

    unsigned int getHeight() const
    {

    }

    QString print();

    bool operator==(const ParsingTree &other) const;
    bool operator!=(const ParsingTree &other) const;

private:
    ParsingTreeNode<T> root;
    unsigned int height;
    T string;

friend class TypeParsingTreeIterator;
friend class TypeParsingTreeNode;

};

#endif // PARSINGTREE_H
