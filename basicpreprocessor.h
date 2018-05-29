#ifndef BASICPREPROCESSOR_H
#define BASICPREPROCESSOR_H

#include "basicprocessor.h"
#include "parsingauxiliarytools.h"

class BasicPreProcessor : public BasicProcessor
{
public:
    BasicPreProcessor(Signature * const signature);

    QString processString(QString string) const;

    QString toString() const;

private:

    TokenStringWrapper wrapTokenString(const QString &string) const;
    void setupAuxiliaryRecords(TokenStringWrapper &tokenString, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const;
    void considerToken(const TokenStringWrapperIterator &tokenIter, QVector<AuxiliaryTokenRecord> &necessaryRecords) const;

    TokenStringWrapperIterator findOperatorLeftParenthesisIterator(const TokenStringWrapper &tokenString,
                                                                   const unsigned int numberOfArgumentsBeforeOperator,
                                                                   const TokenStringWrapperIterator &tokenStringIter) const;
    TokenStringWrapperIterator findOperatorRightParenthesisIterator(const TokenStringWrapper &tokenString,
                                                                    const unsigned int numberOfArgumentsAfterOperator,
                                                                    const TokenStringWrapperIterator &tokenStringIter) const;
    void findDelimiterScopeEndIterator(const TokenStringWrapper &tokenString,
                                       TokenStringWrapperIterator &iter) const;
    void findDelimiterScopeEndReverseIterator(const TokenStringWrapper &tokenString,
                                              TokenStringWrapperReverseIterator &iter) const;
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
