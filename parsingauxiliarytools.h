#ifndef PARSINGAUXILIARYTOOLS_H
#define PARSINGAUXILIARYTOOLS_H

namespace ParsingAuxiliaryTools
{

enum class ParsingDirection
{
    LeftToRight,
    RightToLeft
};

template<class StringClass, class CharClass>
unsigned int findDelimiterScopeEndIndex(const StringClass &sentence,
                                        const CharClass &leftDelimiter,
                                        const CharClass &rightDelimiter,
                                        const unsigned int startPos = 0,
                                        const ParsingDirection direction = ParsingDirection::LeftToRight)
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

        if(direction == ParsingDirection::LeftToRight)
        {
            index++;
        }
        else if(direction == ParsingDirection::RightToLeft)
        {
            index--;
        }
        else
        {
            throw std::logic_error("This shouldn't be happening!");
        }


        if((index == sentence.size() && direction == ParsingDirection::LeftToRight) ||
           (index == 0 && direction == ParsingDirection::RightToLeft))
        {
            if(leftDelimiterCount != rightDelimiterCount)
            {
                throw std::invalid_argument("Delimiters count do not match!");
            }
        }

    } while(leftDelimiterCount != rightDelimiterCount);


    const unsigned int tokenLookaheadCompensation = 1;

    if(direction == ParsingDirection::LeftToRight)
    {
        return index - tokenLookaheadCompensation;
    }
    else
    {
        return index + tokenLookaheadCompensation;
    }
}



}

#endif // PARSINGAUXILIARYTOOLS_H
