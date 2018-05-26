#include "tokenstring.h"

TokenString::TokenString()
{

}

bool TokenString::tokenNeedsSubsequentSeparation(const QVector<Token *> &tokenList, const int index) const
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
    return tokenList.size();
}

TokenString TokenString::mid(const unsigned int beginIndex, const unsigned int size) const
{
    TokenString newString;

    for(unsigned int counter = 0; counter < size; counter++)
    {
        newString.tokenList.push_back(this->tokenList[beginIndex + counter]);
    }

    return newString;
}

//Token &TokenString::operator[](const unsigned int index)
//{
//    return const_cast<Token &>(static_cast<const TokenString &>(*this)[index]);
//}

Token &TokenString::operator[](const unsigned int index) const
{
    if(!indexIsWithinBounds(index))
    {
        throw std::invalid_argument("Index is out of bounds!");
    }

    return *(tokenList[index]);
}

//Token &TokenString::first()
//{
//    return const_cast<Token &>(static_cast<const TokenString &>(*this).first());
//}

Token &TokenString::first() const
{
    if(this->isEmpty())
    {
        throw std::invalid_argument("Token String is empty!");
    }

    return (*this)[0];
}

//Token &TokenString::last()
//{
//    return const_cast<Token &>(static_cast<const TokenString &>(*this).last());
//}

Token &TokenString::last() const
{
    if(this->isEmpty())
    {
        throw std::invalid_argument("Token String is empty!");
    }

    const unsigned int zeroIndexCompensation = 1;

    return (*this)[this->size() - zeroIndexCompensation];
}

TokenString &TokenString::insert(const unsigned int index, const QString &token)
{
    //NOTE This is more complex than it seems, must be tested!
    if(index > size())
    {
        throw std::invalid_argument("Index is out of bounds!");
    }

    const unsigned int insertedTokenCompensation = 1;
    const unsigned int afterInsertListSize = tokenList.size() - index;
    QVector<Token *> afterInsertList = tokenList.mid(index, afterInsertListSize);

    tokenList.resize(tokenList.size() + insertedTokenCompensation);
    tokenList[index] = signature->getTokenPointer(token);
    for(int tokenListIndex = index + insertedTokenCompensation, afterInsertListIndex = 0; afterInsertListIndex < afterInsertList.size(); tokenListIndex++, afterInsertListIndex++)
    {
        tokenList[tokenListIndex] = afterInsertList[afterInsertListIndex];
    }

    return *this;
}

TokenString &TokenString::swapTokens(const unsigned int index1, const unsigned int index2)
{
    if(!indexIsWithinBounds(index1) || !indexIsWithinBounds(index2))
    {
        throw std::invalid_argument("Index is out of bounds!");
    }

    Token *temp = tokenList[index1];
    tokenList[index1] = tokenList[index2];
    tokenList[index2] = temp;

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
