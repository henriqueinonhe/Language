#include "basicprocessor.h"

BasicProcessor::BasicProcessor(Signature * const signature) :
    signature(signature)
{

}

void BasicProcessor::addTokenRecord(const QString &token, const unsigned int position, const BasicProcessorTokenRecord::Associativity associativity, const int precedenceRank)
{
    const CoreToken &coreToken = dynamic_cast<CoreToken &>(*(signature->getTokenPointer(token)));
    const BasicProcessorTokenRecord newRecord(coreToken, position, associativity);
    bool conflictingRecord = std::any_of(tokenRecords.begin(), tokenRecords.end(), [&newRecord](const BasicProcessorTokenRecord &record)
    {
        return newRecord.token == record.token;
    });

    if(conflictingRecord)
    {
        throw std::invalid_argument("There is already a record with this token!");
    }

    if(precedenceRank >= 0)
    {
        auto insertPosition = tokenRecords.begin();
        insertPosition += precedenceRank;
        tokenRecords.insert(insertPosition, newRecord);
    }
    else
    {
        tokenRecords.push_back(newRecord);
    }
}

void BasicProcessor::removeTokenRecord(const QString &tokenString)
{
    bool tokenFound = false;

    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++)
    {
        if(iter->token == tokenString)
        {
            tokenRecords.erase(iter);
            tokenFound = true;
            break;
        }
    }

    if(!tokenFound)
    {
        throw std::invalid_argument("The token prompted to be removed is not in the list!");
    }

}

unsigned int BasicProcessor::getOperatorPrecedenceRank(const QString &tokenString) const
{
    unsigned int precedenceRank = 0;
    bool tokenFound = false;

    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++, precedenceRank++)
    {
        if(iter->token == tokenString)
        {
            tokenFound = true;
            break;
        }
    }

    if(!tokenFound)
    {
        throw std::invalid_argument("There is no record associated with this token!");
    }

    return precedenceRank;
}

unsigned int BasicProcessor::getOperatorPosition(const QString &tokenString) const
{
    unsigned int operatorPosition = 0;
    bool tokenFound = false;

    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++)
    {
        if(iter->token == tokenString)
        {
            operatorPosition = iter->operatorPosition;
            tokenFound = true;
            break;
        }
    }

    if(!tokenFound)
    {
        throw std::invalid_argument("There is no record associated with this token!");
    }

    return operatorPosition;
}

BasicProcessorTokenRecord::Associativity BasicProcessor::getOperatorAssociativity(const QString &tokenString) const
{
    BasicProcessorTokenRecord::Associativity associativity;
    bool tokenFound = false;

    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++)
    {
        if(iter->token == tokenString)
        {
            associativity = iter->associativity;
            tokenFound = true;
            break;
        }
    }

    if(!tokenFound)
    {
        throw std::invalid_argument("There is no record associated with this token!");
    }

    return associativity;
}

const BasicProcessorTokenRecord *BasicProcessor::getTokenRecordPtr(const QString &token) const
{
    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++)
    {
        if(iter->token == token)
        {
            return &(*iter);
        }
    }

    throw std::invalid_argument("There is no record associated with this token!");
}

QString BasicProcessor::tokenStringWrapperToString(BasicProcessor::TokenStringWrapper tokenString) const
{
    QString string;

    for(TokenStringWrapperIterator iter = tokenString.begin(); iter != tokenString.end(); iter++)
    {
        string += iter->token;

        if(tokenNeedsSubsequentSeparation(tokenString, iter))
        {
            string += " ";
        }
    }

    return string;
}

bool BasicProcessor::tokenNeedsSubsequentSeparation(const BasicProcessor::TokenStringWrapper &tokenString, const BasicProcessor::TokenStringWrapperIterator &iter) const
{
    const unsigned int tokenLookaheadCompensation = 1;

    return iter->token != "(" &&
           iter + tokenLookaheadCompensation != tokenString.end() &&
           (iter + tokenLookaheadCompensation)->token != ")";
}

BasicProcessor::TokenStringWrapper BasicProcessor::wrapTokenString(const QString &string) const
{
    TokenStringWrapper tokenString;
    TokenString tempString = Lexer(signature).lex(string);

    for(unsigned int index = 0; index < tempString.size(); index++)
    {
        tokenString.push_back(tempString[index].getString());
    }

    return tokenString;
}

BasicProcessor::TokenStringWrapperIterator BasicProcessor::findDelimiterScopeEndIterator(const BasicProcessor::TokenStringWrapper &tokenString, BasicProcessor::TokenStringWrapperIterator iter) const //NOTE Maybe give a end iterator instead of a tokenString!
{
    unsigned int leftParenthesisCount = 0;
    unsigned int rightParenthesisCount = 0;

    do
    {
        if(iter->token == "(")
        {
            leftParenthesisCount++;
        }

        if(iter->token == ")")
        {
            rightParenthesisCount++;
        }

        iter++;

        if(iter == tokenString.end())
        {
            if(leftParenthesisCount != rightParenthesisCount)
            {
                throw std::invalid_argument("Delimiters count do not match!");
            }
        }
    } while(leftParenthesisCount != rightParenthesisCount);

    const unsigned int tokenLookaheadCompensation = 1;
    iter = iter - tokenLookaheadCompensation;

    return iter;
}

void BasicProcessor::findDelimiterScopeEndReverseIterator(const BasicProcessor::TokenStringWrapper &tokenString, TokenStringWrapperReverseIterator &iter) const
{
    unsigned int leftParenthesisCount = 0;
    unsigned int rightParenthesisCount = 0;

    do
    {
        if(iter->token == "(")
        {
            leftParenthesisCount++;
        }

        if(iter->token == ")")
        {
            rightParenthesisCount++;
        }

        iter++;

        if(iter == tokenString.rend())
        {
            if(leftParenthesisCount != rightParenthesisCount)
            {
                throw std::invalid_argument("Delimiters count do not match!");
            }
        }

    } while(leftParenthesisCount != rightParenthesisCount);

    const unsigned int tokenLookaheadCompensation = 1;
    iter = iter - tokenLookaheadCompensation;
}

BasicProcessor::TokenStringWrapperIterator BasicProcessor::findOperatorLeftParenthesisIterator(const TokenStringWrapper &tokenString, const unsigned int numberOfArgumentsBeforeOperator, const TokenStringWrapperIterator &tokenStringIter) const
{
    unsigned int argumentsBeforeOperatorCount = 0;
    unsigned int reverseIteratorCompensation = 1;
    TokenStringWrapperReverseIterator leftParenthesisIterator(tokenStringIter + reverseIteratorCompensation);
    while(argumentsBeforeOperatorCount != numberOfArgumentsBeforeOperator)
    {
        leftParenthesisIterator++;
        if(leftParenthesisIterator == tokenString.rend())
        {
            throw std::invalid_argument("There are arguments missing!"); //FIXME Needs Specialized Exception!
        }

        if(leftParenthesisIterator->token == ")")
        {
            //Parenthesis Analysis
            try
            {
                findDelimiterScopeEndReverseIterator(tokenString, leftParenthesisIterator);
            }
            catch(const std::invalid_argument &)
            {
                throw std::invalid_argument("Parenthesis doesn't match!"); //FIXME Needs Specialized Exception!
            }

        }
        else if(leftParenthesisIterator->token == "(")
        {
            throw std::invalid_argument("Encountered an unexpected '('!"); //FIXME Needs Specialized Exception!
        }
        leftParenthesisIterator->alreadyScanned = true;
        argumentsBeforeOperatorCount++;
    }

    return (leftParenthesisIterator + reverseIteratorCompensation).base();
}

BasicProcessor::TokenStringWrapperIterator BasicProcessor::findOperatorRightParenthesisIterator(const TokenStringWrapper &tokenString, const unsigned int numberOfArgumentsAfterOperator, const TokenStringWrapperIterator &tokenStringIter) const
{
    unsigned int argumentsAfterOperatorCount = 0;
    TokenStringWrapperIterator rightParenthesisIterator = tokenStringIter;
    while(argumentsAfterOperatorCount != numberOfArgumentsAfterOperator)
    {
        rightParenthesisIterator++;
        if(rightParenthesisIterator == tokenString.end())
        {
            throw std::invalid_argument("Index is out of bounds!"); //FIXME Needs Specialized Exception!
        }

        if(rightParenthesisIterator->token == "(")
        {
            //Parenthesis Analysis
            try
            {
                rightParenthesisIterator = findDelimiterScopeEndIterator(tokenString, rightParenthesisIterator);
            }
            catch(const std::invalid_argument&)
            {
                throw std::invalid_argument("Parenthesis doesn't match!");
            }
        }
        else if(tokenStringIter->token == ")")
        {
            throw std::invalid_argument("Encountered an unexpected ')'!"); //FIXME Needs Specialized Exception!
        }
        else
        {
        }
        argumentsAfterOperatorCount++;
    }

    const unsigned int beforeTokenInsertCompensation = 1;
    return rightParenthesisIterator + beforeTokenInsertCompensation;
}
