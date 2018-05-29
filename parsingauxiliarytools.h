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
                                        unsigned int startPos = 0,
                                        const ParsingDirection direction = ParsingDirection::LeftToRight)
{
    unsigned int leftDelimiterCount = 0;
    unsigned int rightDelimiterCount = 0;
    unsigned int &index = startPos;

    if(direction == ParsingDirection::LeftToRight)
    {
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
            }
        } while(leftDelimiterCount != rightDelimiterCount);


        const unsigned int tokenLookaheadCompensation = 1;
        return index - tokenLookaheadCompensation;
    }
    else if(direction == ParsingDirection::RightToLeft)
    {
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

            index--;

            if(index == 0)
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
