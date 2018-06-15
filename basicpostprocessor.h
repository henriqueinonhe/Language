#ifndef BASICPOSTPROCESSOR_H
#define BASICPOSTPROCESSOR_H

#include "basicprocessor.h"

class BasicPostProcessor : public BasicProcessor
{
public:
    BasicPostProcessor(Signature * const signature);

    QString processString(QString string) const;

    QString toString() const;

protected:
    struct SubSentenceRecord
    {
        TokenStringWrapperIterator operatorIterator;
        TokenStringWrapperIterator firstLeftParenthesisIterator;
        TokenStringWrapperIterator lastRightParenthesisIterator;
        bool isAtomic;

        bool hasAtomicOperator() const
        {
            return operatorIterator->token != "(";
        }

        SubSentenceRecord &operator=(const SubSentenceRecord &other)
        {
            this->operatorIterator = other.operatorIterator;
            this->firstLeftParenthesisIterator = other.firstLeftParenthesisIterator;
            this->lastRightParenthesisIterator = other.lastRightParenthesisIterator;

            return *this;
        }
    };

    void moveOperator(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &mainOperatorIter, const TokenStringWrapperIterator &operatorInsertIter) const;
    void processSubSentence(TokenStringWrapper &tokenString, const SubSentenceRecord &sentenceRecord) const;
    void addSubSentenceRecord(const TokenStringWrapperIterator &tokenStringIter, QVector<SubSentenceRecord> &subSentenceRecords, TokenStringWrapper &tokenString) const;
    bool hasHigherOverallPrecedence(const TokenStringWrapperIterator &subSentenceOperatorIter, const TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const;
    bool hasLowerIndex(const TokenStringWrapperIterator &subSentenceOperatorIter, const TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const;
    void setupSubSentenceRecords(TokenStringWrapper &tokenString, QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &sentenceRecord) const;
    void eraseParenthesis(const TokenStringWrapperIterator &firstLeftParenthesisIter, const TokenStringWrapperIterator &lastRightParenthesisIter, TokenStringWrapper &tokenString) const;
    void parenthesisAnalysis(const QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &mainOperatorRecord, TokenStringWrapper &tokenString, const unsigned int mainOperatorPrecedenceRank, const unsigned int mainOperatorCompensation) const;
};

#endif // BASICPOSTPROCESSOR_H
