#ifndef TOKENSTRING_H
#define TOKENSTRING_H

#include "token.h"
#include "pool.h"

class Lexer;

class TokenString
{
public:

    QString toString() const;

    unsigned int size() const;

    Token operator[](unsigned int index) const;
    Token first() const;
    Token last() const;

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TokenString &other) const;
    bool operator!=(const TokenString &other) const;

private:
    TokenString();

    QVector<Token *> tokenList;

    friend class Lexer;
};

#endif // TOKENSTRING_H
