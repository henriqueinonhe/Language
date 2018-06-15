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


#include <iostream>
QString BasicPreProcessor::processString(QString string) const
{
    //1. Lexing
    TokenStringWrapper tokenString = wrapTokenString(string);

    //2. Auxiliary Records Vector
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

