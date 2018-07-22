#include "lexer.h"

Lexer::Lexer(const Signature * const signature) :
    signature(signature)
{
    if(signature == nullptr)
    {
        throw std::invalid_argument("Signature pointer is null!");
    }
}

void Lexer::lexCoreToken(TokenString &tokenString, int &index, const QString &string) const
{
    const int tokenLookaheadCompensation = 1;
    const int beginTokenIndex = index;
    const int endTokenIndex = findTokenBreakpointIndex(string, beginTokenIndex) - tokenLookaheadCompensation;
    const int tokenSpanCompensation = 1;
    const int tokenSpan = endTokenIndex - beginTokenIndex + tokenSpanCompensation;
    const QString token = string.mid(beginTokenIndex, tokenSpan);

    tokenString.tokenList.push_back(signature->getTokenPointer(token));

    index = endTokenIndex;
}

TokenString Lexer::lex(const QString &string) const
{
    //There are two phases in lexing:
    //1st - Token separation.
    //2st - Type assignment.

    TokenString tokenString;

    for(int index = 0; index < string.size(); index++)
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
    int &index = beginIndex;
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
