#include "basicpreprocessor.h"


BasicPreProcessor::BasicPreProcessor(Signature * const signature) :
    BasicProcessor(signature)
{

}

void BasicPreProcessor::considerToken(const TokenStringWrapperIterator &tokenIter, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const
{
    bool matchingRecordFound = false;
    for(auto record = auxiliaryRecords.begin(); record != auxiliaryRecords.end(); record++)
    {
        if(tokenIter->token == record->record->token)
        {
            matchingRecordFound = true;
            record->tokenIterators.push_back(tokenIter);
            break;
        }
    }

    if(!matchingRecordFound)
    {
        try
        {
            auxiliaryRecords.push_back(AuxiliaryTokenRecord(const_cast<BasicProcessorTokenRecord *>(getTokenRecordPtr(tokenIter->token)), getPrecedenceRank(tokenIter->token)));
            auxiliaryRecords.last().tokenIterators.push_back(tokenIter);
        }
        catch(const std::invalid_argument &)
        {

        }
    }
}

void BasicPreProcessor::setupAuxiliaryRecords(TokenStringWrapper &tokenString, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const
{
    for(TokenStringWrapperIterator tokenIter = tokenString.begin(); tokenIter != tokenString.end(); tokenIter++)
    {
        considerToken(tokenIter, auxiliaryRecords);
    }

    std::sort(auxiliaryRecords.begin(), auxiliaryRecords.end(), [](const AuxiliaryTokenRecord &record1, const AuxiliaryTokenRecord &record2)
    {
        return record1.precedenceRank < record2.precedenceRank;
    });
}


BasicProcessor::TokenStringWrapperIterator BasicPreProcessor::findOperatorLeftParenthesisIterator(const TokenStringWrapper &tokenString, const unsigned int numberOfArgumentsBeforeOperator, const TokenStringWrapperIterator &tokenStringIter) const
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

BasicProcessor::TokenStringWrapperIterator BasicPreProcessor::findOperatorRightParenthesisIterator(const TokenStringWrapper &tokenString, const unsigned int numberOfArgumentsAfterOperator, const TokenStringWrapperIterator &tokenStringIter) const
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
                findDelimiterScopeEndIterator(tokenString, rightParenthesisIterator);
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

void BasicPreProcessor::findDelimiterScopeEndIterator(const BasicProcessor::TokenStringWrapper &tokenString, BasicProcessor::TokenStringWrapperIterator &iter) const
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
}

void BasicPreProcessor::findDelimiterScopeEndReverseIterator(const BasicProcessor::TokenStringWrapper &tokenString, TokenStringWrapperReverseIterator &iter) const
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

bool BasicPreProcessor::operatorParenthesisAreAlreadyPlaced(const TokenStringWrapper &tokenString, const TokenStringWrapperIterator &leftParenthesisIterator, const TokenStringWrapperIterator &rightParenthesisIterator) const
{
    bool leftParenthesisIndexIsStringStart = leftParenthesisIterator == tokenString.begin();
    bool rightParenthesisIndexIsStringEnd = rightParenthesisIterator == tokenString.end();

    if(leftParenthesisIndexIsStringStart || rightParenthesisIndexIsStringEnd)
    {
        return false;
    }
    else
    {
        const unsigned int beforeIteratorInsertCompensation = 1;
        const TokenStringWrapperIterator leftParenthesisActualIterator = leftParenthesisIterator - beforeIteratorInsertCompensation;
        if(leftParenthesisActualIterator->token == "(" &&
           rightParenthesisIterator->token == ")")
        {
            TokenStringWrapperIterator endScopeIterator = ParsingAuxiliaryTools::findDelimiterScopeEndIterator(tokenString,
                                                                                                               TokenWrapper("("),
                                                                                                               TokenWrapper(")"),
                                                                                                               leftParenthesisActualIterator);
            return endScopeIterator == rightParenthesisIterator;
        }
        else
        {
            return false;
        }
    }
}

void BasicPreProcessor::insertOperatorParenthesis(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &leftParenthesisInsertIterator, const TokenStringWrapperIterator &rightParenthesisInsertIterator) const
{
    tokenString.insert(leftParenthesisInsertIterator, TokenWrapper("("));
    tokenString.insert(rightParenthesisInsertIterator, TokenWrapper(")"));
}

void BasicPreProcessor::moveOperator(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &leftParenthesisInsertIterator, const TokenStringWrapperIterator &tokenStringIter) const
{
    const TokenStringWrapperIterator &firstArgumentIterator = leftParenthesisInsertIterator;
    tokenString.insert(firstArgumentIterator, *tokenStringIter);
    tokenString.erase(tokenStringIter);
}

void BasicPreProcessor::processToken(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &tokenStringIter, const AuxiliaryTokenRecord &record) const
{
    //Backwards Scanning
    const unsigned int numberOfArgumentsBeforeOperator = record.record->operatorPosition;
    const TokenStringWrapperIterator leftParenthesisInsertIterator = findOperatorLeftParenthesisIterator(tokenString, numberOfArgumentsBeforeOperator, tokenStringIter);

    //Afterwards Scanning
    const unsigned int numberOfArgumentsAfterOperator = record.record->numberOfArguments - numberOfArgumentsBeforeOperator;
    const TokenStringWrapperIterator rightParenthesisInsertIterator = findOperatorRightParenthesisIterator(tokenString, numberOfArgumentsAfterOperator, tokenStringIter);

    if(!operatorParenthesisAreAlreadyPlaced(tokenString, leftParenthesisInsertIterator, rightParenthesisInsertIterator))
    {
        insertOperatorParenthesis(tokenString, leftParenthesisInsertIterator, rightParenthesisInsertIterator);
    }

    //Put operator on the right place
    moveOperator(tokenString, leftParenthesisInsertIterator, tokenStringIter);
}

BasicProcessor::TokenStringWrapper BasicPreProcessor::wrapTokenString(const QString &string) const
{
    TokenStringWrapper tokenString;
    TokenString tempString = Lexer(signature).lex(string);

    for(unsigned int index = 0; index < tempString.size(); index++)
    {
        tokenString.push_back(tempString[index].getString());
    }

    return tokenString;
}

#include <iostream>
QString BasicPreProcessor::processString(QString string) const
{
    //1. Lexing
    TokenStringWrapper tokenString = wrapTokenString(string);

    //2. Auxiliary Vector
    QVector<AuxiliaryTokenRecord> auxiliaryRecords;
    setupAuxiliaryRecords(tokenString, auxiliaryRecords);

    //3. Processing
    std::for_each(auxiliaryRecords.begin(), auxiliaryRecords.end(), [&tokenString, this](const AuxiliaryTokenRecord &record)
    {
        if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Left)
        {
            std::for_each(record.tokenIterators.begin(), record.tokenIterators.end(), [&tokenString, this, &record](const TokenStringWrapperIterator &tokenStringIter)
            {
                if(!tokenStringIter->alreadyScanned)
                {
                    processToken(tokenString, tokenStringIter, record);
                }
            });
        }
        else if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Right)
        {
            std::for_each(record.tokenIterators.rbegin(), record.tokenIterators.rend(), [&tokenString, this, &record](const TokenStringWrapperIterator &tokenStringIter)
            {
                if(!tokenStringIter->alreadyScanned)
                {
                    processToken(tokenString, tokenStringIter, record);
                }
            });
        }
        else
        {
            throw std::logic_error("This shouldn't be happening!");
        }
    });

    std::cout << tokenStringWrapperToString(tokenString).toStdString();

    return tokenStringWrapperToString(tokenString);
}

QString BasicPreProcessor::toString() const
{
    return "Basic PreProcessor (Operator position, precendece and associativity)";
}

