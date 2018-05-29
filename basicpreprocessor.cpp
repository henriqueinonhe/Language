#include "basicpreprocessor.h"


BasicPreProcessor::BasicPreProcessor(Signature * const signature) :
    BasicProcessor(signature)
{

}

void BasicPreProcessor::considerToken(const TokenWrapper &currentToken, QVector<AuxiliaryTokenRecord> &necessaryRecords) const
{
    unsigned int currentTokenPrecedenceRank = 0;
    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++, currentTokenPrecedenceRank++)
    {
        if(currentToken.token == iter->token)
        {
            necessaryRecords.push_back(AuxiliaryTokenRecord(const_cast<BasicProcessorTokenRecord *>(&(*iter)), currentTokenPrecedenceRank));
        }
    }
}

bool BasicPreProcessor::tokenMatchesRecordAndHasntBeenScanned(const TokenStringWrapperIterator &tokenStringIter, const BasicProcessor::AuxiliaryTokenRecord &record) const
{
    return tokenStringIter->token == record.record->token &&
           !tokenStringIter->alreadyScanned;
}

void BasicPreProcessor::setupAuxiliaryRecords(const TokenStringWrapper &tokenString, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const
{
    std::for_each(tokenString.begin(), tokenString.end(), [&auxiliaryRecords, this](const TokenWrapper &currentToken)
    {
        if(!tokenAlreadyConsidered(currentToken, auxiliaryRecords))
        {
            considerToken(currentToken, auxiliaryRecords);
        }
    });

    std::sort(auxiliaryRecords.begin(), auxiliaryRecords.end(), [](const AuxiliaryTokenRecord &record1, const AuxiliaryTokenRecord &record2)
    {
        return record1.precedenceRank < record2.precedenceRank;
    });
}


BasicPreProcessor::TokenStringWrapperIterator BasicPreProcessor::findOperatorLeftParenthesisIterator(const TokenStringWrapper &tokenString, const unsigned int numberOfArgumentsBeforeOperator, const TokenStringWrapperIterator &tokenStringIter) const
{
    unsigned int argumentsBeforeOperatorCount = 0;
    TokenStringWrapperReverseIterator tokenStringReverseIter(tokenStringIter);
    while(argumentsBeforeOperatorCount != numberOfArgumentsBeforeOperator)
    {
        tokenStringReverseIter++;
        if(tokenStringReverseIter == tokenString.rend())
        {
            throw std::invalid_argument("There are arguments missing!"); //FIXME Needs Specialized Exception!
        }

        if(tokenStringReverseIter->token == ")")
        {
            //Parenthesis Analysis
            try
            {
                tokenStringReverseIter = ParsingAuxiliaryTools::findDelimiterScopeEndReverseIterator(tokenString,
                                                                                                     TokenWrapper("("),
                                                                                                     TokenWrapper(")"),
                                                                                                     tokenStringReverseIter);
            }
            catch(const std::invalid_argument &)
            {
                throw std::invalid_argument("Parenthesis doesn't match!"); //FIXME Needs Specialized Exception!
            }

        }
        else if(tokenStringReverseIter->token == "(")
        {
            throw std::invalid_argument("Encountered an unexpected '('!"); //FIXME Needs Specialized Exception!
        }
        argumentsBeforeOperatorCount++;
    }

    return tokenStringReverseIter.base();
}

BasicPreProcessor::TokenStringWrapperIterator BasicPreProcessor::findOperatorRightParenthesisIterator(const TokenStringWrapper &tokenString, const unsigned int numberOfArgumentsAfterOperator, TokenStringWrapperIterator tokenStringIter) const
{
    unsigned int argumentsAfterOperatorCount = 0;
    while(argumentsAfterOperatorCount != numberOfArgumentsAfterOperator)
    {
        tokenStringIter++;
        if(tokenStringIter == tokenString.end())
        {
            throw std::invalid_argument("Index is out of bounds!"); //FIXME Needs Specialized Exception!
        }

        if(tokenStringIter->token == "(")
        {
            //Parenthesis Analysis
            try
            {
                tokenStringIter = ParsingAuxiliaryTools::findDelimiterScopeEndIterator(tokenString,
                                                                                       TokenWrapper("("),
                                                                                       TokenWrapper(")"),
                                                                                       tokenStringIter);
            }
            catch(const std::invalid_argument&)
            {
                throw std::invalid_argument("Parenthesis doesn't match!");
            }
            tokenStringIter++;
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
    return tokenStringIter + beforeTokenInsertCompensation;
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
        if((leftParenthesisIterator - beforeIteratorInsertCompensation)->token == "(" &&
           rightParenthesisIterator->token == ")")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void BasicPreProcessor::insertOperatorParenthesis(TokenStringWrapper &tokenString, TokenStringWrapperIterator leftParenthesisInsertIterator, TokenStringWrapperIterator rightParenthesisInsertIterator) const
{
    tokenString.insert(leftParenthesisInsertIterator, TokenWrapper("("));
    tokenString.insert(rightParenthesisInsertIterator, TokenWrapper(")"));
}

void BasicPreProcessor::moveOperator(const TokenStringWrapperIterator &leftParenthesisInsertIterator, TokenStringWrapperIterator &tokenStringIter) const
{
    const TokenStringWrapperIterator &firstArgumentIterator = leftParenthesisInsertIterator;
    std::swap<TokenWrapper>(*firstArgumentIterator, *tokenStringIter);
}

void BasicPreProcessor::processToken(TokenStringWrapper &tokenString, TokenStringWrapperIterator &tokenStringIter, const AuxiliaryTokenRecord &record) const
{
    //Backwards Scanning
    const unsigned int numberOfArgumentsBeforeOperator = record.record->operatorPosition;
    const TokenStringWrapperIterator leftParenthesisInsertIterator = findOperatorLeftParenthesisIterator(tokenString, numberOfArgumentsBeforeOperator, tokenStringIter);

    //Afterwards Scanning
    const unsigned int numberOfArgumentsAfterOperator = record.record->numberOfArguments - numberOfArgumentsBeforeOperator;
    const TokenStringWrapperIterator rightParenthesisInsertIterator = findOperatorRightParenthesisIterator(tokenString, numberOfArgumentsAfterOperator, tokenStringIter);

    if(!operatorParenthesisAreAlreadyPlaced(tokenString, leftParenthesisInsertIterator, rightParenthesisInsertIterator))
    {
        insertOperatorParenthesis(tokenString, rightParenthesisInsertIterator, leftParenthesisInsertIterator);
    }

    //Put operator on the right place
    moveOperator(leftParenthesisInsertIterator, tokenStringIter);
}

QLinkedList<BasicProcessor::TokenWrapper> BasicPreProcessor::wrapTokenString(const QString &string) const
{
    TokenStringWrapper tokenString;
    TokenString tempString = Lexer(signature).lex(string);

    for(unsigned int index = 0; index < tempString.size(); index++)
    {
        tokenString.push_back(tempString[index].getString());
    }

    return tokenString;
}

QString BasicPreProcessor::processString(QString string) const
{
    //Here begins the real fun!

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
            for(TokenStringWrapperIterator tokenStringIter = tokenString.begin(); tokenStringIter != tokenString.end(); tokenStringIter++)
            {
                if(tokenMatchesRecordAndHasntBeenScanned(tokenStringIter, record))
                {
                    processToken(tokenString, tokenStringIter, record);
                }
            }
        }
        else if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Right)
        {
            for(TokenStringWrapperReverseIterator tokenStringRevIter = tokenString.rbegin(); tokenStringRevIter != tokenString.rend(); tokenStringRevIter++)
            {
                TokenStringWrapperIterator tokenStringIter = tokenStringRevIter.base();
                if(tokenMatchesRecordAndHasntBeenScanned(tokenStringIter, record))
                {
                    processToken(tokenString, tokenStringIter, record);
                }
            }
        }
        else
        {
            throw std::logic_error("This shouldn't be happening!");
        }
    });

    //return tokenString.toString();
}

QString BasicPreProcessor::toString() const
{
    return "Basic PreProcessor (Operator position, precendece and associativity)";
}

bool BasicPreProcessor::tokenAlreadyConsidered(const TokenWrapper &tokenWrapper, const QVector<AuxiliaryTokenRecord> &necessaryRecords) const
{
    return std::any_of(necessaryRecords.begin(), necessaryRecords.end(), [&tokenWrapper](const AuxiliaryTokenRecord &record)
    {
        return tokenWrapper.token == record.record->token;
    });
}
