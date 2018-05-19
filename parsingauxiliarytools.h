#ifndef PARSINGAUXILIARYTOOLS_H
#define PARSINGAUXILIARYTOOLS_H

namespace ParsingAuxiliaryTools
{

template<class StringClass, class CharClass>
unsigned int findDelimiterScopeEndIndex(const StringClass &sentence,
                                        const CharClass &leftDelimiter,
                                        const CharClass &rightDelimiter,
                                        const unsigned int startPos = 0)
{
    unsigned int leftDelimiterCount = 0;
    unsigned int rightDelimiterCount = 0;
    unsigned int index = startPos;

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
                throw std::invalid_argument("Delimiters count do not match!");
            }
//            else
//            {
//                throw std::invalid_argument("The sentence ends exactly at the delimiter!");
//            } This check should be done by other means!
        }

    } while(leftDelimiterCount != rightDelimiterCount);


    const unsigned int tokenLookaheadCompensation = 1;

    return index - tokenLookaheadCompensation;
}



}

#endif // PARSINGAUXILIARYTOOLS_H
