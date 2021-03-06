#ifndef BASICPREPROCESSOR_H
#define BASICPREPROCESSOR_H

#include "basicprocessor.h"
#include "parsingauxiliarytools.h"

class BasicPreProcessor : public BasicProcessor
{
public:
    BasicPreProcessor(const Signature * const signature);
    BasicPreProcessor(QDataStream &stream, const Signature * const signature);

    QString processString(const QString &string) const override;

    QString toString() const override;

protected:
    void serialize(QDataStream &stream) const override;
    void deserialize(QDataStream &stream) override;

private:
    struct AuxiliaryTokenRecord
    {
        AuxiliaryTokenRecord(){}
        AuxiliaryTokenRecord(BasicProcessorTokenRecord * record,
                       const unsigned int precedenceRank) :
            record(record),
            precedenceRank(precedenceRank)
        {}

        BasicProcessorTokenRecord *record;
        unsigned int precedenceRank;
        QVector<QLinkedList<TokenWrapper>::iterator> tokenIterators;
    };

    void setupAuxiliaryRecords(TokenStringWrapper &tokenString, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const;
    void considerToken(const TokenStringWrapperIterator &tokenIter, QVector<AuxiliaryTokenRecord> &auxilaryRecords) const;
    bool operatorParenthesisAreAlreadyPlaced(const TokenStringWrapper &tokenString,
                                             const TokenStringWrapperIterator &leftParenthesisIterator,
                                             const TokenStringWrapperIterator &rightParenthesisIterator) const;
    void insertOperatorParenthesis(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &rightParenthesisInsertIterator,
                                   const TokenStringWrapperIterator &rightParenthesisInsertIndex) const;
    void moveOperator(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &leftParenthesisInsertIterator,
                      const TokenStringWrapperIterator &tokenStringIter) const;
    void processToken(TokenStringWrapper &tokenString, const TokenStringWrapperIterator &tokenStringIter, const AuxiliaryTokenRecord &record) const;


};

#endif // BASICPREPROCESSOR_H
