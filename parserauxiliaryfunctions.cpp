#include "parserauxiliaryfunctions.h"

unsigned int ParserAuxiliaryFunctions::findMainOperatorIndexWithDelimiters(const QVector<TypeToken> &sentence, const TypeToken &leftDelimiter, const TypeToken &rightDelimiter)
{
    unsigned int leftDelimiterCount = 0;
    unsigned int rightDelimiterCount = 0;
    unsigned int index = 0;

    do
    {
        if(sentence[index] == leftDelimiter)
        {
            leftDelimiterCount++;
        }

        if(sentence[index] == rightDelimiter)
        {
            rightDelimiterCount++;
        }

        index++;

        if(index == sentence.size())
        {
            if(leftDelimiterCount != rightDelimiterCount)
            {
                throw std::invalid_argument("Delimiters do not match!");
            }
            else
            {
                throw std::invalid_argument("The sentence ends exactly at the delimiter!");
            }
        }

    } while(leftDelimiterCount != rightDelimiterCount);

    return index;
}
