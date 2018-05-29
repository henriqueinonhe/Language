#ifndef BASICPREPROCESSOR_H
#define BASICPREPROCESSOR_H

#include "basicprocessor.h"
#include "parsingauxiliarytools.h"

class BasicPreProcessor : public BasicProcessor
{
public:
    typedef QLinkedList<TokenWrapper> TokenStringWrapper;
    typedef QLinkedList<TokenWrapper>::iterator TokenStringWrapperIterator;
    typedef QLinkedList<TokenWrapper>::reverse_iterator TokenStringWrapperReverseIterator;

    BasicPreProcessor(Signature * const signature);

    QString processString(QString string) const;

    QString toString() const;

private:

    QLinkedList<TokenWrapper> wrapTokenString(const QString &string) const;
    void setupAuxiliaryRecords(const TokenStringWrapper &tokenString, QVector<AuxiliaryTokenRecord> &auxiliaryRecords) const;
    bool tokenAlreadyConsidered(const TokenWrapper &token, const QVector<AuxiliaryTokenRecord> &necessaryRecords) const;
    void considerToken(const TokenWrapper &currentToken, QVector<AuxiliaryTokenRecord> &necessaryRecords) const;

    bool tokenMatchesRecordAndHasntBeenScanned(const TokenStringWrapperIterator &tokenStringIter, const AuxiliaryTokenRecord &record) const;
    TokenStringWrapperIterator findOperatorLeftParenthesisIterator(const TokenStringWrapper &tokenString,
                                                                const unsigned int numberOfArgumentsBeforeOperator,
                                                                const TokenStringWrapperIterator &tokenStringIter) const;
    TokenStringWrapperIterator findOperatorRightParenthesisIterator(const TokenStringWrapper &tokenString,
                                                                 const unsigned int numberOfArgumentsAfterOperator,
                                                                 TokenStringWrapperIterator tokenStringIter) const;
    bool operatorParenthesisAreAlreadyPlaced(const TokenStringWrapper &tokenString,
                                             const TokenStringWrapperIterator &leftParenthesisIterator,
                                             const TokenStringWrapperIterator &rightParenthesisIterator) const;
    void insertOperatorParenthesis(TokenStringWrapper &tokenString, TokenStringWrapperIterator rightParenthesisInsertIterator,
                                   TokenStringWrapperIterator rightParenthesisInsertIndex) const;
    void moveOperator(const TokenStringWrapperIterator &leftParenthesisInsertIterator,
                      TokenStringWrapperIterator &tokenStringIter) const;
    void processToken(TokenStringWrapper &tokenString, TokenStringWrapperIterator &tokenStringIter, const AuxiliaryTokenRecord &record) const;
};

#endif // BASICPREPROCESSOR_H
