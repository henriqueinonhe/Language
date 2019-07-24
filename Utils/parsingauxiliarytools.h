#ifndef PARSINGAUXILIARYTOOLS_H
#define PARSINGAUXILIARYTOOLS_H

#include <stdexcept>

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
                                        unsigned int startIndex,
                                        unsigned int endIndex,
                                        const ParsingDirection direction = ParsingDirection::LeftToRight)
{
    unsigned int leftDelimiterCount = 0;
    unsigned int rightDelimiterCount = 0;
    unsigned int &index = startIndex;
    const unsigned int zeroIndexCompensation = 1;

    if(direction == ParsingDirection::LeftToRight)
    {
        const unsigned int subSentenceSize = endIndex + zeroIndexCompensation;
        do
        {
            if(sentence[index] == leftDelimiter)
            {
                leftDelimiterCount++;
            }
            else if(sentence[index] == rightDelimiter)
            {
                rightDelimiterCount++;
            }

            index++;

            if(index == subSentenceSize)
            {
                if(leftDelimiterCount != rightDelimiterCount)
                {
                    throw std::invalid_argument("Delimiters count do not match!");
                }
            }
        } while(leftDelimiterCount != rightDelimiterCount);


        const unsigned int tokenLookaheadCompensation = 1;
        return index - tokenLookaheadCompensation;
    }
    else if(direction == ParsingDirection::RightToLeft)
    {
        const unsigned int subSentenceBeforeFirstIndex = endIndex - zeroIndexCompensation;
        do
        {
            if(sentence[index] == leftDelimiter)
            {
                leftDelimiterCount++;
            }
            else if(sentence[index] == rightDelimiter)
            {
                rightDelimiterCount++;
            }

            index--;

            if(index == subSentenceBeforeFirstIndex)
            {
                if(leftDelimiterCount != rightDelimiterCount)
                {
                    throw std::invalid_argument("Delimiters count do not match!");
                }
            }
        } while(leftDelimiterCount != rightDelimiterCount);


        const unsigned int tokenLookaheadCompensation = 1;
        return index + tokenLookaheadCompensation;
    }
    else
    {
        throw std::logic_error("This shouldn't be happening!");
    }
}

template<typename StringClass, typename CharClass>
typename StringClass::iterator findDelimiterScopeEndIterator(const StringClass &sentence,
                                                             const CharClass &leftDelimiter,
                                                             const CharClass &rightDelimiter,
                                                             typename StringClass::iterator iter)
{
    unsigned int leftDelimiterCount = 0;
    unsigned int rightDelimiterCount = 0;

    do
    {
        if((*iter) == leftDelimiter)
        {
            leftDelimiterCount++;
        }

        if((*iter) == rightDelimiter)
        {
            rightDelimiterCount++;
        }

        iter++;

        if(iter == sentence.end())
        {
            if(leftDelimiterCount != rightDelimiterCount)
            {
                throw std::invalid_argument("Delimiters count do not match!");
            }
        }
    } while(leftDelimiterCount != rightDelimiterCount);


    const unsigned int tokenLookaheadCompensation = 1;
    return iter - tokenLookaheadCompensation;
}

template<typename StringClass, typename CharClass>
typename StringClass::reverse_iterator findDelimiterScopeEndReverseIterator(const StringClass &sentence,
                                                                            const CharClass &leftDelimiter,
                                                                            const CharClass &rightDelimiter,
                                                                            typename StringClass::reverse_iterator iter)
{
    unsigned int leftDelimiterCount = 0;
    unsigned int rightDelimiterCount = 0;

    do
    {
        if((*iter) == leftDelimiter)
        {
            leftDelimiterCount++;
        }

        if((*iter) == rightDelimiter)
        {
            rightDelimiterCount++;
        }

        iter++;

        if(iter == sentence.rend())
        {
            if(leftDelimiterCount != rightDelimiterCount)
            {
                throw std::invalid_argument("Delimiters count do not match!");
            }
        }

    } while(leftDelimiterCount != rightDelimiterCount);

    const unsigned int tokenLookaheadCompensation = 1;
    return iter - tokenLookaheadCompensation;
}


}

#endif // PARSINGAUXILIARYTOOLS_H
