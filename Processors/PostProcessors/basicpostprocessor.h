#ifndef BASICPOSTPROCESSOR_H
#define BASICPOSTPROCESSOR_H

#include "basicprocessor.h"

class BasicPostProcessor : public BasicProcessor
{
public:
    BasicPostProcessor();
    BasicPostProcessor(const Signature * const signature);

    QString processString(const QString &string) const override;

    QString toString() const override;

protected:
    void serialize(QDataStream &stream) const override;
    void deserialize(QDataStream &stream) override;

    struct SubSentenceRecord
    {
        TokenStringWrapperIterator operatorIterator;
        TokenStringWrapperIterator firstLeftParenthesisIterator;
        TokenStringWrapperIterator lastRightParenthesisIterator;
        bool isAtomicSentence;

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
    void processSubSentence(TokenStringWrapper &tokenString, const SubSentenceRecord &mainSentenceRecord) const;
    void addSubSentenceRecord(TokenStringWrapperIterator &subSentenceScannerIter, QVector<SubSentenceRecord> &subSentenceRecords, const TokenStringWrapper &tokenString) const;
    bool hasHigherOverallPrecedence(const TokenStringWrapperIterator &subSentenceOperatorIter, const TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const;
    bool hasLowerIndex(const TokenStringWrapperIterator &subSentenceOperatorIter, const TokenStringWrapperIterator &mainSentenceOperatorIter, const TokenStringWrapper &tokenString) const;
    void setupSubSentenceRecords(TokenStringWrapper &tokenString, QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &mainSentenceRecord) const;
    void eraseParenthesis(const TokenStringWrapperIterator &firstLeftParenthesisIter, const TokenStringWrapperIterator &lastRightParenthesisIter, TokenStringWrapper &tokenString) const;
    void parenthesisAnalysis(const QVector<SubSentenceRecord> &subSentenceRecords, const SubSentenceRecord &mainOperatorRecord, TokenStringWrapper &tokenString) const;
    void setupFirstRecord(SubSentenceRecord &firstRecord, TokenStringWrapper &tokenString) const;
    void operatorPositionAnalysis(TokenStringWrapper &tokenString, SubSentenceRecord &mainOperatorRecord) const;
    unsigned int getOperatorArity(const SubSentenceRecord &mainSentenceRecord) const;
    bool subSentenceIsAtomic(const TokenStringWrapperIterator &firstSubSentenceToken) const;
};

#endif // BASICPOSTPROCESSOR_H
