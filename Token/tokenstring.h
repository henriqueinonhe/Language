#ifndef TOKENSTRING_H
#define TOKENSTRING_H

#include "token.h"
#include "signature.h"

class Lexer;
class Parser;

class TokenString
{
public:
    TokenString();

    QString toString() const;
    QString formattedString() const;

    unsigned int size() const;

    TokenString mid(const unsigned int beginIndex, const unsigned int size) const;

    const Token &operator[](const unsigned int index) const;
    const Token &first() const;
    const Token &last() const;

//    TokenString &insert(const unsigned int index, const QString &token); //Deprecated
//    TokenString &swapTokens(const unsigned int index1, const unsigned int index2);//Deprecated

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TokenString &other) const;
    bool operator!=(const TokenString &other) const;

private:

    //Token &operator[](const unsigned int index);
    //Token &first();
    //Token &last();

    bool tokenNeedsSubsequentSeparation(const QVector<const Token *> &tokenList, const int index) const;

    QVector<const Token *> tokenList;
    const Signature *signature;

    friend class Lexer;
};

#endif // TOKENSTRING_H
