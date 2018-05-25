#include "lexer.h"

Lexer::Lexer(Signature * const signature) :
    signature(signature)
{
    if(signature == nullptr)
    {
        throw std::invalid_argument("Signature pointer is null!");
    }
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
            const int tokenLookaheadCompensation = 1;
            const int beginIndex = index;
            const int endIndex = findTokenBreakpointIndex(string, beginIndex) - tokenLookaheadCompensation;
            const int tokenSpan = endIndex - beginIndex + 1;
            const QString token = string.mid(beginIndex, tokenSpan);

            tokenString.tokenList.push_back(signature->getTokenPointer(token));

            index = endIndex;
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
