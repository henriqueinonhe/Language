#ifndef TOKENSTRING_H
#define TOKENSTRING_H

#include <QVector>

class QString;
class QDataStream;
class Signature;
class Token;

class Lexer;
class Parser;

class TokenString
{
public:
    TokenString();
    TokenString(QDataStream &stream, Signature * const signature);

    QString toString() const;
    QString formattedString() const;

    unsigned int size() const;

    TokenString mid(const unsigned int beginIndex, const unsigned int size) const;

    const Token &operator[](const unsigned int index) const;
    const Token &first() const;
    const Token &last() const;

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TokenString &other) const;
    bool operator!=(const TokenString &other) const;

private:
    bool tokenNeedsSubsequentSeparation(const QVector<const Token *> &tokenList, const int index) const;

    QVector<const Token *> tokenList;

    friend class Lexer;
    friend QDataStream &operator << (QDataStream &stream, const TokenString &tokenString);
};

QDataStream &operator << (QDataStream &stream, const TokenString &tokenString);
QDataStream &operator >> (QDataStream &stream, TokenString &tokenString);

#endif // TOKENSTRING_H
