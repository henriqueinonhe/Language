#ifndef LEXER_H
#define LEXER_H

#include "signature.h"
#include "tokenstring.h"
#include "punctuationtoken.h"
#include "coretoken.h"

class Lexer
{
public:
    Lexer(Signature *signature);
    TokenString lex(const QString &string);

private:
    bool isSeparator(const QChar &character) const;
    int findTokenBreakpointIndex(const QString &string, const int beginIndex) const;

    Signature *signature;
};

#endif // LEXER_H
