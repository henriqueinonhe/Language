#ifndef PARSINGERROREXCEPTION_HPP
#define PARSINGERROREXCEPTION_HPP

#include <stdexcept>
#include <QString>
#include <string>

using namespace std;

template <class T>
class ParsingErrorException : public std::invalid_argument
{
public:
    ParsingErrorException(const char *str,
                          const unsigned int errorBeginIndex,
                          const unsigned int errorEndIndex,
                          const T &errorString) :
        std::invalid_argument(str),
        errorBeginIndex(errorBeginIndex),
        errorEndIndex(errorEndIndex),
        errorString(errorString),
        explanatoryString(str)
    {
    }

    virtual const string what() noexcept
    {
        const QString explanatoryQString(this->explanatoryString);
        const QString errorQString = errorString.toString();
        const QString hightlight = formatHighlight();

        QString whatString = explanatoryQString + "\n" +
                             errorQString + "\n" +
                             hightlight;

        return whatString.toStdString();
    }

private:
    QString formatHighlight()
    {
        QString highlight;

        for(unsigned int tokenIndex = 0; tokenIndex < errorBeginIndex; tokenIndex++)
        {
            for(unsigned int charIndex = 0; charIndex < errorString[tokenIndex].getTokenCharacterSpan(); charIndex++)
            {
                highlight += " ";
            }
        }

        for(unsigned int tokenIndex = errorBeginIndex; tokenIndex <= errorEndIndex; tokenIndex++)
        {
            for(unsigned int charIndex = 0; charIndex < errorString[tokenIndex].getTokenCharacterSpan(); charIndex++)
            {
                highlight += "^";
            }
        }

        const unsigned int zeroIndexCompensation = 1;

        if(errorEndIndex != errorString.size() - zeroIndexCompensation)
        {
            for(unsigned int tokenIndex = errorEndIndex + 1; tokenIndex < errorString.size() - zeroIndexCompensation; tokenIndex++)
            {
                for(unsigned int charIndex = 0; charIndex < errorString[tokenIndex].getTokenCharacterSpan(); charIndex++)
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
