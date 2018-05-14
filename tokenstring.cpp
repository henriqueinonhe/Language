#include "tokenstring.h"

TokenString::TokenString()
{

}

unsigned int TokenString::size() const
{
    return tokenList.size();
}

TokenString TokenString::mid(const unsigned int startPos, const unsigned int n) const //TODO
{
}

Token TokenString::operator[](unsigned int index) const
{
    if(!indexIsWithinBounds(index))
    {
        throw std::invalid_argument("Index is out of bounds!");
    }

    return *(tokenList[index]);
}

Token TokenString::first() const
{
    if(this->isEmpty())
    {
        throw std::invalid_argument("Token String is empty!");
    }

    return (*this)[0];
}

Token TokenString::last() const
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
