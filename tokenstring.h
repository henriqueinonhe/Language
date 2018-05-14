#ifndef TOKENSTRING_H
#define TOKENSTRING_H

#include "token.h"

class TokenString
{
public:

    TokenString();

    QString toString() const; //TODO

    unsigned int size() const;
    TokenString mid(const unsigned int startPos, const unsigned int n) const;

    Token operator[](unsigned int index) const;
    Token first() const;
    Token last() const;

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TokenString &other) const;
    bool operator!=(const TokenString &other) const;

private:


    QVector<PoolRecordPointer<Token>> tokenList;
};

#endif // TOKENSTRING_H
