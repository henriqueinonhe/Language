#include "basicpostprocessor.h"
#include "signature.h"

BasicPostProcessor::BasicPostProcessor() : BasicProcessor (nullptr)
{

}

BasicPostProcessor::BasicPostProcessor(const Signature * const signature) :
    BasicProcessor(signature)
{

}

void BasicPostProcessor::setupFirstRecord(SubSentenceRecord &firstRecord, TokenStringWrapper &tokenString) const
{
    firstRecord.firstLeftParenthesisIterator = tokenString.begin();
    firstRecord.lastRightParenthesisIterator = tokenString.end();
    const auto parenthesisCompensation = 1;
    firstRecord.operatorIterator = tokenString.begin() + parenthesisCompensation;
}

QString BasicPostProcessor::processString(const QString &string) const
{
    //Sanity Check
    if(signature == nullptr)
    {
        throw invalid_argument("The signature hasn't been set up yet!");
    }

    //1. Lexing
    auto tokenString = wrapTokenString(string);

    //2. Processing
    SubSentenceRecord firstRecord;
    setupFirstRecord(firstRecord, tokenString);

    processSubSentence(tokenString, firstRecord);

    //Tira o parentesis mais externo
    tokenString.pop_front();
    tokenString.pop_back();

    return tokenStringWrapperToString(tokenString);
}

QString BasicPostProcessor::toString() const
{
    return "Basic PostProcessor (Operator position, precedence and associativity)";
}

void BasicPostProcessor::serialize(QDataStream &stream) const
{
    //TODO
}

void BasicPostProcessor::deserialize(QDataStream &stream)
{
    //TODO
}

void BasicPostProcessor::moveOperator(BasicProcessor::TokenStringWrapper &tokenString, const BasicProcessor::TokenStringWrapperIterator &mainOperatorIter, const BasicProcessor::TokenStringWrapperIterator &operatorInsertIter) const
{
    tokenString.insert(operatorInsertIter, *mainOperatorIter);
    tokenString.erase(mainOperatorIter);
}

void BasicPostProcessor::addSubSentenceRecord(TokenStringWrapperIterator &subSentenceScannerIter, QVector<SubSentenceRecord> &subSentenceRecords, const TokenStringWrapper &tokenString) const
{
    SubSentenceRecord subSentenceRecord;
    if(!subSentenceIsAtomic(subSentenceScannerIter))
    {
        const auto nextTokenCompensation = 1;

        subSentenceRecord.firstLeftParenthesisIterator = subSentenceScannerIter;
        subSentenceRecord.operatorIterator = subSentenceScannerIter + nextTokenCompensation;
        subSentenceRecord.lastRightParenthesisIterator = findDelimiterScopeEndIterator(tokenString, subSentenceScannerIter);
        subSentenceRecord.isAtomicSentence = false;

        subSentenceScannerIter = subSentenceRecord.lastRightParenthesisIterator + nextTokenCompensation;
    }
    else
    {
        subSentenceRecord.operatorIterator = subSentenceScannerIter;
        subSentenceRecord.isAtomicSentence = true;

        subSentenceScannerIter++;
    }

    subSentenceRecords.push_back(subSentenceRecord);
}

bool BasicPostProcessor::hasHigherOverallPrecedence(const BasicProcessor::TokenStringWrapperIterator &subSentenceOperatorIter, const BasicProcessor::TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const //FIXME Fix iterator end
{
    const auto subSentenceOperatorPrecedenceRank = getOperatorPrecedenceRank(subSentenceOperatorIter->token);
    const auto mainSentenceOperatorPrecedenceRank = getOperatorPrecedenceRank(mainSentenceOperatorIter->token);
    if(subSentenceOperatorPrecedenceRank < mainSentenceOperatorPrecedenceRank)
    {
        return true;
    }
    else if(subSentenceOperatorPrecedenceRank == mainSentenceOperatorPrecedenceRank)
    {
        //Associativity Check
        if(getOperatorAssociativity(mainSentenceOperatorIter->token) == BasicProcessorTokenRecord::Associativity::Left)
        {
            return hasLowerIndex(subSentenceOperatorIter, mainSentenceOperatorIter, tokenString);
        }
        else
        {
            return !hasLowerIndex(subSentenceOperatorIter, mainSentenceOperatorIter, tokenString);
        }
    }
    else
    {
        return false;
    }
}

bool BasicPostProcessor::hasLowerIndex(const BasicProcessor::TokenStringWrapperIterator &subSentenceOperatorIter, const BasicProcessor::TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const
{
    for(auto tokenStringIter = subSentenceOperatorIter; tokenStringIter != tokenString.end(); tokenStringIter++)
    {
        if(tokenStringIter == mainSentenceOperatorIter)
        {
            return true;
        }
    }

    return false;
}

unsigned int BasicPostProcessor::getOperatorArity(const SubSentenceRecord &mainSentenceRecord) const
{
    //NOTE Make sure this is actually a core token!
    const auto operatorArity = dynamic_cast<const CoreToken *>(signature->getTokenPointer(mainSentenceRecord.operatorIterator->token))->getType().getNumberOfArguments();

    return operatorArity;
}

bool BasicPostProcessor::subSentenceIsAtomic(const BasicProcessor::TokenStringWrapperIterator &firstSubSentenceToken) const
{
    return firstSubSentenceToken->token != "(";
}

void BasicPostProcessor::setupSubSentenceRecords(TokenStringWrapper &tokenString, QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &mainSentenceRecord) const
{
    if(mainSentenceRecord.hasAtomicOperator())
    {
        const auto mainSentenceOperatorArity = getOperatorArity(mainSentenceRecord);
        const auto mainSentenceOperatorCompensation = 1;
        auto subSentenceScannerIter = mainSentenceRecord.operatorIterator;

        for(unsigned int subSentenceCount = 0; subSentenceCount < mainSentenceOperatorArity + mainSentenceOperatorCompensation; subSentenceCount++)
        {
            addSubSentenceRecord(subSentenceScannerIter, subSentenceRecords, tokenString);
        }
    }
    else
    {
        const auto parenthesisCompensation = 1;
        const auto mainSentenceFirstTokenIterator = mainSentenceRecord.firstLeftParenthesisIterator + parenthesisCompensation;
        const auto mainSentenceLastTokenIterator = mainSentenceRecord.lastRightParenthesisIterator;

        for(auto subSentenceScannerIter = mainSentenceFirstTokenIterator; subSentenceScannerIter != mainSentenceLastTokenIterator;)
        {
            addSubSentenceRecord(subSentenceScannerIter, subSentenceRecords, tokenString);
        }
    }
}

void BasicPostProcessor::eraseParenthesis(const BasicProcessor::TokenStringWrapperIterator &firstLeftParenthesisIter, const BasicProcessor::TokenStringWrapperIterator &lastRightParenthesisIter, TokenStringWrapper &tokenString) const
{
    tokenString.erase(firstLeftParenthesisIter);
    tokenString.erase(lastRightParenthesisIter);
}

void BasicPostProcessor::parenthesisAnalysis(const QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &mainOperatorRecord, TokenStringWrapper &tokenString) const
{   
    const auto mainOperatorCompensation = 1;
    if(mainOperatorRecord.hasAtomicOperator())
    {
        std::for_each(subSentenceRecords.begin() + mainOperatorCompensation, subSentenceRecords.end(), [&mainOperatorRecord, &tokenString, this](const SubSentenceRecord &record)
        {
            if(!record.isAtomicSentence &&
               record.hasAtomicOperator() &&
               hasHigherOverallPrecedence(record.operatorIterator, mainOperatorRecord.operatorIterator, tokenString))
            {
                eraseParenthesis(record.firstLeftParenthesisIterator, record.lastRightParenthesisIterator, tokenString);
            }
        });
    }
    else
    {
        std::for_each(subSentenceRecords.begin() + mainOperatorCompensation, subSentenceRecords.end(), [ &tokenString, this](const SubSentenceRecord &record)
        {
            if(!record.isAtomicSentence &&
               record.hasAtomicOperator())
            {
                eraseParenthesis(record.firstLeftParenthesisIterator, record.lastRightParenthesisIterator, tokenString);
            }
        });
    }
}

void BasicPostProcessor::operatorPositionAnalysis(TokenStringWrapper &tokenString, SubSentenceRecord &mainOperatorRecord) const
{
    if(mainOperatorRecord.hasAtomicOperator())
    {
        const auto mainOperatorPosition = getOperatorPosition(mainOperatorRecord.operatorIterator->token);
        const auto mainOperatorInsertIterator = findOperatorRightParenthesisIterator(tokenString,
                                                                                                           mainOperatorPosition,
                                                                                                           mainOperatorRecord.operatorIterator);
        moveOperator(tokenString, mainOperatorRecord.operatorIterator, mainOperatorInsertIterator);

        const auto insertedOperatorCompensation = 1;
        mainOperatorRecord.operatorIterator = mainOperatorInsertIterator - insertedOperatorCompensation;
    }
}

void BasicPostProcessor::processSubSentence(TokenStringWrapper &tokenString, const SubSentenceRecord &mainSentenceRecord) const
{
    /* Em sequência:
     * 1. Montar lista com os iteradores dos argumentos
     * 2. Mudar o operador de lugar
     * 3. Tirar os parenteses quando possível
     * 4. Repetir o processo recursivamente com os filhos (argumentos). */

    // 1 - Montando lista de Iteradores dos argumentos
    QVector<SubSentenceRecord> subSentenceRecords;
    setupSubSentenceRecords(tokenString, subSentenceRecords, mainSentenceRecord);

    // 2 - Mudar operador de lugar
    auto &mainOperatorRecord = subSentenceRecords.first();
    operatorPositionAnalysis(tokenString, mainOperatorRecord);

    // 3 - Tirar os parênteses quando possível

    parenthesisAnalysis(subSentenceRecords, mainOperatorRecord, tokenString);

    // 4 - Recursando tudo
    for(const auto &record : subSentenceRecords)
    {
        if(!record.isAtomicSentence)
        {
            processSubSentence(tokenString, record);
        }
    }
}
