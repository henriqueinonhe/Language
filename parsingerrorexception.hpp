#ifndef PARSINGERROREXCEPTION_HPP
#define PARSINGERROREXCEPTION_HPP

#include <stdexcept>
#include <QString>

template <class T>
class ParsingErrorException : public std::invalid_argument
{
public:
    ParsingErrorException(const char *str,
                          const unsigned int errorBeginIndex,
                          const unsigned int errorEndIndex,
                          T errorString) :
        std::invalid_argument(str),
        errorBeginIndex(errorBeginIndex),
        errorEndIndex(errorEndIndex),
        errorString(errorString),
        explanatoryString(str)
    {
        //NOTE Maybe should take some precautionary measures with indexes and error string size!
    }

    virtual const char *what() noexcept
    {
        const QString explanatoryQString(this->explanatoryString);
        const QString errorQString = errorString.toString();
        const QString hightlight = formatHighlight();

        QString whatString = explanatoryQString + "\n" +
                             errorQString + "\n" +
                             hightlight;

        return whatString.toLatin1().data();
    }

private:
    QString formatHighlight()
    {
        QString highlight;

        for(unsigned int tokenIndex = 0; tokenIndex < errorBeginIndex; tokenIndex++)
        {
            for(unsigned int charIndex = 0; charIndex < errorString[tokenIndex].getTokenCharSpan(); charIndex++)
            {
                highlight += " ";
            }
        }

        for(unsigned int tokenIndex = errorBeginIndex; tokenIndex <= errorEndIndex; tokenIndex++)
        {
            for(unsigned int charIndex = 0; charIndex < errorString[tokenIndex].getTokenCharSpan(); charIndex++)
            {
                highlight += "^";
            }
        }

        const unsigned int zeroIndexCompensation = 1;

        if(errorEndIndex != errorString.size() - zeroIndexCompensation)
        {
            for(unsigned int tokenIndex = errorEndIndex + 1; tokenIndex < errorString.size() - zeroIndexCompensation; tokenIndex++)
            {
                for(unsigned int charIndex = 0; charIndex < errorString[tokenIndex].getTokenCharSpan(); charIndex++)
                {
                    highlight += " ";
                }
            }

        }

        return highlight;

    }

    unsigned int errorBeginIndex;
    unsigned int errorEndIndex;
    T errorString;
    const char *explanatoryString;
};

#endif // PARSINGERROREXCEPTION_HPP
