#ifndef TOKENSTRING_H
#define TOKENSTRING_H

#include "token.h"
#include "pool.h"

class Lexer;
class Parser;

class TokenString
{
public:
    QString toString() const;
    QString formattedString() const;

    unsigned int size() const;

    TokenString mid(const unsigned int beginIndex, const unsigned int size) const;

    Token &operator[](const unsigned int index);
    Token &operator[](const unsigned int index) const;
    Token &first();
    Token &first() const;
    Token &last();
    Token &last() const;

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TokenString &other) const;
    bool operator!=(const TokenString &other) const;

private:
    TokenString();
    bool tokenNeedsSubsequentSeparation(const QVector<Token *> &tokenList, const int index) const;

    QVector<Token *> tokenList;

    friend class Lexer;
};

#endif // TOKENSTRING_H
