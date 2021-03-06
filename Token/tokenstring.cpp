#include "tokenstring.h"
#include "token.h"
#include "lexer.h"
#include <QDataStream>

TokenString::TokenString(QDataStream &stream, Signature * const signature) :
    tokenList(deserializeTokenList(stream, signature))
{
}

bool TokenString::tokenNeedsSubsequentSeparation(const QVector<const Token *> &tokenList, const int index) const
{
    const auto tokenLookaheadCompensation = 1;

    return tokenList[index]->getString() != "(" &&
           index < tokenList.size() - tokenLookaheadCompensation &&
           tokenList[index + tokenLookaheadCompensation]->getString() != ")";
}

QString TokenString::toString() const
{
    QString string;
    for(const auto token : tokenList)
    {
        string += token->getString();
    }

    return string;
}

QString TokenString::formattedString() const
{
    //NOTE Document difference between toString and formattedString
    QString string;
    for(auto index = 0; index < tokenList.size(); index++)
    {
        string += tokenList[index]->getString();

        if(tokenNeedsSubsequentSeparation(tokenList, index))
        {
            string += " ";
        }
    }

    return string;
}

unsigned int TokenString::size() const
{
    return static_cast<unsigned int>(tokenList.size());
}

TokenString TokenString::mid(const unsigned int beginIndex, const unsigned int size) const
{
    TokenString newString;

    for(unsigned int counter = 0; counter < size; counter++)
    {
        newString.tokenList.push_back(this->tokenList[static_cast<int>(beginIndex + counter)]);
    }

    return newString;
}

const Token &TokenString::operator[](const unsigned int index) const
{
    if(!indexIsWithinBounds(index))
    {
        throw std::invalid_argument("Index is out of bounds!");
    }

    return *(tokenList[static_cast<int>(index)]);
}

const Token &TokenString::first() const
{
    if(this->isEmpty())
    {
        throw std::invalid_argument("Token String is empty!");
    }

    return (*this)[0];
}

const Token &TokenString::last() const
{
    if(this->isEmpty())
    {
        throw std::invalid_argument("Token String is empty!");
    }

    const auto zeroIndexCompensation = 1;

    return (*this)[this->size() - zeroIndexCompensation];
}

TokenString &TokenString::replace(const Token &before, const Token *after)
{
    for(auto &token : tokenList)
    {
        if(*token == before)
        {
            token = after;
        }
    }
    return *this;
}

bool TokenString::indexIsWithinBounds(const unsigned int index) const
{
    return index < this->size();
}

bool TokenString::isEmpty() const
{
    return this->size() == 0;
}

bool TokenString::operator==(const TokenString &other) const
{
    if(this->tokenList.size() == other.tokenList.size())
    {
        for(auto index = 0; index < this->tokenList.size(); index++)
        {
            if(*this->tokenList[index] != *other.tokenList[index])
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool TokenString::operator!=(const TokenString &other) const
{
    return !(*this == other);
}

QVector<const Token *> TokenString::deserializeTokenList(QDataStream &stream, Signature * const signature) const
{
    Lexer lexer(signature);
    QString formattedString;
    stream >> formattedString;

    return lexer.lex(formattedString).tokenList;
}

QDataStream &operator <<(QDataStream &stream, const TokenString &tokenString)
{
    stream << tokenString.formattedString();

    return stream;
}
