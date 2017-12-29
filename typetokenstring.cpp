#include "typetokenstring.h"

Pool<TypeToken> TypeTokenString::pool;

TypeTokenString::TypeTokenString(const QString &string)
{
    lexString(string);
}

QString TypeTokenString::toString() const
{
    QString str;

    std::for_each(tokenList.begin(), tokenList.end(), [&str](const PoolRecordPointer<TypeToken> &record) {
        str += record->getString();
    });

    return str;
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
        else if(string[index] == '{')
        {
            tokenList.push_back(pool.getPointer(TypeToken("{")));
        }
        else if(string[index] == '}')
        {
            tokenList.push_back(pool.getPointer(TypeToken("}")));
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
        else
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
           c == "{" ||
           c == "}" ||
           c == "," ||
           c == "-";
}
