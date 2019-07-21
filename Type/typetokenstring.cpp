#include "typetokenstring.h"

Pool<TypeToken> TypeTokenString::pool;

TypeTokenString::TypeTokenString(QDataStream &stream) :
    tokenList(deserializeTokenList(stream))
{
}

TypeTokenString::TypeTokenString(const QString &string) :
    tokenList(lexString(string))
{
}

QString TypeTokenString::toString() const
{
    QString str;
    for(const auto &tokenRecord : tokenList)
    {
        str += tokenRecord->getString();
    }

    return str;
}

unsigned int TypeTokenString::size() const
{
    return static_cast<unsigned int>(tokenList.size());
}

TypeTokenString TypeTokenString::mid(const unsigned int startPos, const unsigned int n) const
{
    return TypeTokenString(tokenList.mid(static_cast<int>(startPos), static_cast<int>(n)));
}

TypeToken TypeTokenString::operator[](unsigned int index) const
{
    if(!indexIsWithinBounds(index))
    {
        throw std::invalid_argument("Index out of bounds.");
    }

    return *tokenList[static_cast<int>(index)];
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

    return *tokenList[static_cast<int>(size()) - 1];
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

bool TypeTokenString::operator==(const TypeTokenString &other) const
{
    if(this->size() == other.size())
    {
        for(int index = 0; index < tokenList.size(); index++)
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

bool TypeTokenString::operator!=(const TypeTokenString &other) const
{
    return !(*this == other);
}

QVector<PoolRecordPointer<TypeToken> > TypeTokenString::deserializeTokenList(QDataStream &stream)
{
    //TODO Implement PoolRecordPointer serialization so I can get rid of this
    QString stringnizedTypeString;
    stream >> stringnizedTypeString;

    return lexString(stringnizedTypeString);
}

TypeTokenString::TypeTokenString(const QVector<PoolRecordPointer<TypeToken> > &tokenList) :
    tokenList(tokenList)
{

}

void TypeTokenString::lexCompositionOperator(const QString &string, int &index)
{
    /* The composition operator is "->", so it must check
     * whether the operator is complete.
     * The '-' character triggers this function. */

    index++;
    if(index < string.size() && string[index] == '>') //Size must be checked first, as string may have ended prematurely
    {
        tokenList.push_back(pool.getPointer(TypeToken("->")));
    }
    else
    {
        throw std::invalid_argument("The composition operator is not complete.");
    }
}

void TypeTokenString::lexPrimitiveTypeToken(const QString &string, int &index)
{
    /* Finds the extent of the primitive type token, that is, it keeps
     * on scanning until it finds a separator. */

    const auto startIndex = index;
    while(index < string.size() && !characterIsSeparator(string[index])) //Size must be checked first, as string may have ended already
    {
        index++;
    }

    tokenList.push_back(pool.getPointer(TypeToken(string.mid(startIndex, index - startIndex))));

    const auto tokenLookaheadCompensation = 1;
    index -= tokenLookaheadCompensation;
}

QVector<PoolRecordPointer<TypeToken> > TypeTokenString::lexString(const QString &string)
{
    QVector<PoolRecordPointer<TypeToken>> tokenList;
    for(auto index = 0; index < string.size(); index++)
    {
        if(string[index] == '(')
        {
            tokenList.push_back(pool.getPointer(TypeToken("(")));
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
            lexCompositionOperator(string, index);
        }
        else
        {
            lexPrimitiveTypeToken(string, index);
        }
    }

    return tokenList;
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

QDataStream &operator <<(QDataStream &stream, const TypeTokenString &string)
{
    stream << string.toString();

    return stream;
}

QDataStream &operator >>(QDataStream &stream, TypeTokenString &string)
{
    string.tokenList = string.deserializeTokenList(stream);

    return stream;
}
