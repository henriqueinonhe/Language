﻿#include "typetokenstring.h"

Pool<TypeToken> TypeTokenString::pool;

TypeTokenString::TypeTokenString(const QString &string)
{
    lexString(string);
}

QString TypeTokenString::toString() const
{
    QString str;

    std::for_each(tokenList.begin(), tokenList.end(), [&str](const PoolRecordPointer<TypeToken> &tokenRecord) {
        str += tokenRecord->getString();
    });

    return str;
}

unsigned int TypeTokenString::size() const
{
    return tokenList.size();
}

TypeTokenString TypeTokenString::mid(const unsigned int startPos, const unsigned int n) const
{
    return TypeTokenString(tokenList.mid(startPos, n));
}

TypeToken TypeTokenString::operator[](unsigned int index) const
{
    if(!indexIsWithinBounds(index))
    {
        throw std::invalid_argument("Index out of bounds.");
    }

    return *tokenList[index];
}

TypeToken TypeTokenString::first() const
{
    if(isEmpty())
    {
        throw std::invalid_argument("String is empty!");
    }

    return *tokenList[0];
}

TypeToken TypeTokenString::last() const
{
    if(isEmpty())
    {
        throw std::invalid_argument("String is empty!");
    }

    return *tokenList[size() - 1];
}

bool TypeTokenString::isLastIndex(const unsigned int index) const
{
    return index == size() - 1;
}

bool TypeTokenString::indexIsWithinBounds(const unsigned int index) const
{
    return index <= size() - 1;
}

bool TypeTokenString::isEmpty() const
{
    return size() == 0;
}

TypeTokenString::TypeTokenString(const QVector<PoolRecordPointer<TypeToken> > &tokenList) :
    tokenList(tokenList)
{

}

void TypeTokenString::lexString(const QString &string)
{
    for(int index = 0; index < string.size(); index++)
    {
        if(string[index] == '(')
        {
            tokenList.push_back(pool.getPointer(TypeToken(QString("("))));
        }
        else if(string[index] == ')')
        {
            tokenList.push_back(pool.getPointer(TypeToken(")")));
        }
        else if(string[index] == '[')
        {
            tokenList.push_back(pool.getPointer(TypeToken("[")));
        }
        else if(string[index] == ']')
        {
            tokenList.push_back(pool.getPointer(TypeToken("]")));
        }
        else if(string[index] == ',')
        {
            tokenList.push_back(pool.getPointer(TypeToken(",")));
        }
        else if(string[index] == '-')
        {
            index++;
            if(string[index] == '>')
            {
                tokenList.push_back(pool.getPointer(TypeToken("->")));
            }
            else
            {
                throw std::invalid_argument("The composition operator is not complete.");
            }
        }
        else //TODO explain this shit!
        {
            int startIndex = index;

            while(index < string.size() && !characterIsSeparator(string[index]))
            {
                index++;
            }

            tokenList.push_back(pool.getPointer(TypeToken(string.mid(startIndex, index - startIndex))));

            index--;
        }
    }
}

bool TypeTokenString::characterIsSeparator(const QString &c) const
{
    return c == "(" ||
           c == ")" ||
           c == "[" ||
           c == "]" ||
           c == "," ||
           c == "-";
}
