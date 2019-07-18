#include "lexer.h"
#include "signature.h"
#include <QString>
#include "tokenstring.h"

Lexer::Lexer(Signature * const signature) :
    signature(signature)
{
    if(signature == nullptr)
    {
        throw std::invalid_argument("Signature pointer is null!");
    }
}

void Lexer::lexCoreToken(TokenString &tokenString, int &index, const QString &string) const
{
    const auto tokenLookaheadCompensation = 1;
    const auto beginTokenIndex = index;
    const auto endTokenIndex = findTokenBreakpointIndex(string, beginTokenIndex) - tokenLookaheadCompensation;
    const auto tokenSpanCompensation = 1;
    const auto tokenSpan = endTokenIndex - beginTokenIndex + tokenSpanCompensation;
    const auto token = string.mid(beginTokenIndex, tokenSpan);

    tokenString.tokenList.push_back(signature->getTokenPointer(token));

    index = endTokenIndex;
}

Signature *Lexer::getSignature() const
{
    return signature;
}

TokenString Lexer::lex(const QString &string) const
{
    //There are two phases in lexing:
    //1st - Token separation.
    //2st - Type assignment.

    TokenString tokenString;
    for(auto index = 0; index < string.size(); index++)
    {
        if(string[index] == '(')
        {
            tokenString.tokenList.push_back(signature->getTokenPointer("("));
        }
        else if(string[index] == ')')
        {
            tokenString.tokenList.push_back(signature->getTokenPointer(")"));
        }
        else if(string[index] == ' ')
        {
            continue;
        }
        else
        {
            lexCoreToken(tokenString, index, string);
        }
    }

    return tokenString;
}

bool Lexer::isSeparator(const QChar &character) const
{
    return character == '(' ||
           character == ')' ||
           character == ' ';
}

bool Lexer::stringHasEnded(const QString &string, const int index) const
{
    return index >= string.size();
}

int Lexer::findTokenBreakpointIndex(const QString &string, int beginIndex) const
{
    auto &index = beginIndex;
    while(!isSeparator(string[index]))
    {
        index++;

        if(stringHasEnded(string, index))
        {
            break;
        }
    }

    return index;
}
