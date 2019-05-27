#include "tokenstring.h"
#include "token.h"
#include "lexer.h"
#include <QDataStream>

TokenString::TokenString()
{

}

TokenString::TokenString(QDataStream &stream, Signature * const signature)
{
    Lexer lexer(signature);
    QString formattedString;
    stream >> formattedString;

    *this = lexer.lex(formattedString);
}

bool TokenString::tokenNeedsSubsequentSeparation(const QVector<const Token *> &tokenList, const int index) const
{
    const int tokenLookaheadCompensation = 1;

    return tokenList[index]->getString() != "(" &&
           index < tokenList.size() - tokenLookaheadCompensation &&
           tokenList[index + tokenLookaheadCompensation]->getString() != ")";
}

QString TokenString::toString() const
{
    QString string;

    for(int index = 0; index < tokenList.size(); index++)
    {
        string += tokenList[index]->getString();
    }

    return string;
}

QString TokenString::formattedString() const
{
    //NOTE Document difference between toString and formattedString
    QString string;

    for(int index = 0; index < tokenList.size(); index++)
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

    const unsigned int zeroIndexCompensation = 1;

    return (*this)[this->size() - zeroIndexCompensation];
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
        for(int index = 0; index < this->tokenList.size(); index++)
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

QDataStream &operator <<(QDataStream &stream, const TokenString &tokenString)
{
    stream << tokenString.formattedString();

    return stream;
}
