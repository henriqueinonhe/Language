#include "basicpostprocessor.h"

BasicPostProcessor::BasicPostProcessor(Signature * const signature) :
    BasicProcessor(signature)
{

}

QString BasicPostProcessor::processString(QString string) const
{
    //1. Lexing
    TokenStringWrapper tokenString = wrapTokenString(string);

    //2. Processing
    SubSentenceRecord firstRecord;
    firstRecord.firstLeftParenthesisIterator = tokenString.begin();
    firstRecord.lastRightParenthesisIterator = tokenString.end();
    const unsigned int parenthesisCompensation = 1;
    firstRecord.operatorIterator = tokenString.begin() + parenthesisCompensation;

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

void BasicPostProcessor::moveOperator(BasicProcessor::TokenStringWrapper &tokenString, const BasicProcessor::TokenStringWrapperIterator &mainOperatorIter, const BasicProcessor::TokenStringWrapperIterator &operatorInsertIter) const
{
    tokenString.insert(operatorInsertIter, *mainOperatorIter);
    tokenString.erase(mainOperatorIter);
}

void BasicPostProcessor::addSubSentenceRecord(const TokenStringWrapperIterator &tokenStringIter, QVector<SubSentenceRecord> &subSentenceRecords, TokenStringWrapper &tokenString) const
{
    SubSentenceRecord newRecord;
    if(tokenStringIter->token == "(")
    {
        const unsigned int nextTokenCompensation = 1;

        newRecord.firstLeftParenthesisIterator = tokenStringIter;
        newRecord.operatorIterator = tokenStringIter + nextTokenCompensation;
        newRecord.lastRightParenthesisIterator = findDelimiterScopeEndIterator(tokenString, tokenStringIter);
        newRecord.isAtomic = false;
    }
    else
    {
        newRecord.operatorIterator = tokenStringIter;
        newRecord.isAtomic = true;
    }

    subSentenceRecords.push_back(newRecord);
}

bool BasicPostProcessor::hasHigherOverallPrecedence(const BasicProcessor::TokenStringWrapperIterator &subSentenceOperatorIter, const BasicProcessor::TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const //FIXME Fix iterator end
{
    const unsigned int mainSentenceOperatorPrecedenceRank = getOperatorPrecedenceRank(mainSentenceOperatorIter->token);
    const unsigned int subSentenceOperatorPrecedenceRank = getOperatorPrecedenceRank(subSentenceOperatorIter->token);
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
    for(TokenStringWrapperIterator tokenStringIter = subSentenceOperatorIter; subSentenceOperatorIter != tokenString.end(); tokenStringIter++)
    {
        if(tokenStringIter == mainSentenceOperatorIter)
        {
            return true;
        }
    }

    return false;
}

void BasicPostProcessor::setupSubSentenceRecords(TokenStringWrapper &tokenString, QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &sentenceRecord) const
{
    if(sentenceRecord.hasAtomicOperator())
    {
        const unsigned int operatorArity = dynamic_cast<CoreToken *>(signature->getTokenPointer(sentenceRecord.operatorIterator->token))->getType().getNumberOfArguments();
        for(unsigned int iterIncrement = 0; iterIncrement < operatorArity; iterIncrement++)
        {
            TokenStringWrapperIterator tokenStringIter = sentenceRecord.operatorIterator + iterIncrement;
            addSubSentenceRecord(tokenStringIter, subSentenceRecords, tokenString);
        }
    }
    else
    {
        const unsigned int parenthesisCompensation = 1;
        const TokenStringWrapperIterator firstTokenIterator = sentenceRecord.firstLeftParenthesisIterator + parenthesisCompensation;
        const TokenStringWrapperIterator lastTokenIterator = sentenceRecord.lastRightParenthesisIterator;
        for(TokenStringWrapperIterator tokenStringIter = firstTokenIterator; tokenStringIter != lastTokenIterator; tokenStringIter++)
        {
            addSubSentenceRecord(tokenStringIter, subSentenceRecords, tokenString);
        }
    }
}

void BasicPostProcessor::eraseParenthesis(const BasicProcessor::TokenStringWrapperIterator &firstLeftParenthesisIter, const BasicProcessor::TokenStringWrapperIterator &lastRightParenthesisIter, TokenStringWrapper &tokenString) const
{
    tokenString.erase(firstLeftParenthesisIter);
    tokenString.erase(lastRightParenthesisIter);
}

void BasicPostProcessor::parenthesisAnalysis(const QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &mainOperatorRecord, TokenStringWrapper &tokenString, const unsigned int mainOperatorPrecedenceRank, const unsigned int mainOperatorCompensation) const
{
    if(mainOperatorRecord.hasAtomicOperator())
    {
        std::for_each(subSentenceRecords.begin() + mainOperatorCompensation, subSentenceRecords.end(), [&mainOperatorRecord, mainOperatorPrecedenceRank, &tokenString, this](const SubSentenceRecord &record)
        {
            if(record.hasAtomicOperator() && hasHigherOverallPrecedence(record.operatorIterator, mainOperatorRecord.operatorIterator, tokenString))
            {
                eraseParenthesis(record.firstLeftParenthesisIterator, record.lastRightParenthesisIterator, tokenString);
            }
        });
    }
    else
    {
        std::for_each(subSentenceRecords.begin() + mainOperatorCompensation, subSentenceRecords.end(), [&mainOperatorRecord, mainOperatorPrecedenceRank, &tokenString, this](const SubSentenceRecord &record)
        {
            if(record.hasAtomicOperator())
            {
                eraseParenthesis(record.firstLeftParenthesisIterator, record.lastRightParenthesisIterator, tokenString);
            }
        });
    }
}

void BasicPostProcessor::processSubSentence(TokenStringWrapper &tokenString, const SubSentenceRecord &sentenceRecord) const
{
    /* Em sequência:
     * 1. Montar lista com os iteradores dos argumentos
     * 2. Mudar o operador de lugar
     * 3. Tirar os parenteses quando possível
     * 4. Repetir o processo recursivamente com os filhos (argumentos). */

    // 1 - Montando lista de Iteradores dos argumentos
    QVector<SubSentenceRecord> subSentenceRecords;
    setupSubSentenceRecords(tokenString, subSentenceRecords, sentenceRecord);

    // 2 - Mudar operador de lugar
    SubSentenceRecord &mainOperatorRecord = subSentenceRecords.first();
    if(mainOperatorRecord.hasAtomicOperator())
    {
        const unsigned int mainOperatorPosition = getOperatorPosition(mainOperatorRecord.operatorIterator->token);
        const TokenStringWrapperIterator mainOperatorInsertIterator = findOperatorRightParenthesisIterator(tokenString,
                                                                                                           mainOperatorPosition,
                                                                                                           mainOperatorRecord.operatorIterator);
        moveOperator(tokenString, mainOperatorRecord.operatorIterator, mainOperatorInsertIterator); //FIXME Probably refactor!
        const unsigned int insertedOperatorCompensation = 1;
        mainOperatorRecord.operatorIterator = mainOperatorInsertIterator + insertedOperatorCompensation;
    }

    //Tem que consertar o iterador depois q ele se move!

    // 3 - Tirar os parênteses quando possível
    const unsigned int mainOperatorCompensation = 1;
    const unsigned int mainOperatorPrecedenceRank = getOperatorPrecedenceRank(mainOperatorRecord.operatorIterator->token);
    parenthesisAnalysis(subSentenceRecords, mainOperatorRecord, tokenString, mainOperatorPrecedenceRank, mainOperatorCompensation);

    // 4 - Recursando tudo
    std::for_each(subSentenceRecords.begin(), subSentenceRecords.end(), [&tokenString, this](const SubSentenceRecord &record)
    {
        if(!record.isAtomic)
        {
            processSubSentence(tokenString, record);
        }
    });

}
