#ifndef LEXER_H
#define LEXER_H

#include "signature.h"
#include "tokenstring.h"
#include "punctuationtoken.h"
#include "coretoken.h"

class Lexer
{
public:
    Lexer(const Signature * const signature);
    TokenString lex(const QString &string) const;

private:
    bool isSeparator(const QChar &character) const;
    bool stringHasEnded(const QString &string, const int index) const;
    int findTokenBreakpointIndex(const QString &string, int beginIndex) const;

    const Signature *signature;
    void lexCoreToken(TokenString &tokenString, int &index, const QString &string) const;
};

#endif // LEXER_H
