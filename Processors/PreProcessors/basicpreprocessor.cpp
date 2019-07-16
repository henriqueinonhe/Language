#include "basicpreprocessor.h"


BasicPreProcessor::BasicPreProcessor(Signature * const signature) :
    BasicProcessor(signature)
{

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
        const auto beforeIteratorInsertCompensation = 1;
        const auto leftParenthesisActualIterator = leftParenthesisIterator - beforeIteratorInsertCompensation;
        if(leftParenthesisActualIterator->token == "(" &&
           rightParenthesisIterator->token == ")")
        {
            auto endScopeIterator = ParsingAuxiliaryTools::findDelimiterScopeEndIterator(tokenString,
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
    const auto &firstArgumentIterator = leftParenthesisInsertIterator;
    tokenString.insert(firstArgumentIterator, *tokenStringIter);
    tokenString.erase(tokenStringIter);
}

void BasicPreProcessor::processToken(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &tokenStringIter, const AuxiliaryTokenRecord &record) const
{
    //Right to Left Scanning
    const unsigned int numberOfArgumentsBeforeOperator = record.record->findTokenSubRecord(tokenStringIter->token).operatorPosition;
    const auto leftParenthesisInsertIterator = findOperatorLeftParenthesisIterator(tokenString, numberOfArgumentsBeforeOperator, tokenStringIter);

    //Left to Right Scanning
    const unsigned int numberOfArgumentsAfterOperator = record.record->findTokenSubRecord(tokenStringIter->token).numberOfArguments - numberOfArgumentsBeforeOperator;
    const auto rightParenthesisInsertIterator = findOperatorRightParenthesisIterator(tokenString, numberOfArgumentsAfterOperator, tokenStringIter);

    if(!operatorParenthesisAreAlreadyPlaced(tokenString, leftParenthesisInsertIterator, rightParenthesisInsertIterator))
    {
        insertOperatorParenthesis(tokenString, leftParenthesisInsertIterator, rightParenthesisInsertIterator);
    }

    //Put operator on the right place
    moveOperator(tokenString, leftParenthesisInsertIterator, tokenStringIter);
}


QString BasicPreProcessor::processString(const QString &string) const
{
    //Sanity Check
    if(signature == nullptr)
    {
        throw invalid_argument("Signature has not been set up!");
    }

    //1. Lexing
    auto tokenString = wrapTokenString(string);

    //2. Auxiliary Records Vector
    QVector<AuxiliaryTokenRecord> auxiliaryRecords;
    setupAuxiliaryRecords(tokenString, auxiliaryRecords);

    //3. Processing
    for(const auto &record : auxiliaryRecords)
    {
        if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Left)
        {
            for(const auto &tokenStringIter : record.tokenIterators)
            {
                if(!tokenStringIter->alreadyScanned)
                {
                    processToken(tokenString, tokenStringIter, record);
                }
            }
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
    }


    return tokenStringWrapperToString(tokenString);
}

QString BasicPreProcessor::toString() const
{
    return "Basic PreProcessor (Operator position, precendece and associativity)";
}

void BasicPreProcessor::setupAuxiliaryRecords(TokenStringWrapper &tokenString, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const
{
    for(auto tokenIter = tokenString.begin(); tokenIter != tokenString.end(); tokenIter++)
    {
        considerToken(tokenIter, auxiliaryRecords);
    }

    std::sort(auxiliaryRecords.begin(), auxiliaryRecords.end(), [](const AuxiliaryTokenRecord &record1, const AuxiliaryTokenRecord &record2)
    {
        return record1.precedenceRank < record2.precedenceRank;
    });
}



void BasicPreProcessor::considerToken(const TokenStringWrapperIterator &tokenIter, QVector<AuxiliaryTokenRecord> &auxilaryRecords) const
{
    bool matchingRecordFound = false;//NOTE Make it's own function
    for(auto record = auxilaryRecords.begin(); record != auxilaryRecords.end(); record++)
    {
        if(record->record->hasTokenAsSubRecord(tokenIter->token))
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
            auxilaryRecords.push_back(AuxiliaryTokenRecord(const_cast<BasicProcessorTokenRecord *>(getTokenRecordPtr(tokenIter->token)), getOperatorPrecedenceRank(tokenIter->token)));
            auxilaryRecords.last().tokenIterators.push_back(tokenIter);
        }
        catch(const std::invalid_argument &)
        {
            //FIXME What should be done here?
        }
    }
}


BasicPreProcessor::BasicPreProcessor() : BasicProcessor (nullptr)
{

}
