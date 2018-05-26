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
    bool tokenAlreadyConsidered(const Token &token, const QVector<EnhancedRecord> &necessaryRecords) const;
    void considerToken(const Token &currentToken, QVector<EnhancedRecord> &necessaryRecords) const;
    void setupNecessaryRecords(const TokenString &tokenString, QVector<EnhancedRecord> &necessaryRecords) const;

    unsigned int findOperatorLeftParenthesisIndex(const TokenString &tokenString,
                                                  const unsigned int numberOfArgumentsBeforeOperator,
                                                  const unsigned int tokenIndex) const;
    unsigned int findOperatorRightParenthesisIndex(const TokenString &tokenString,
                                                   const unsigned int numberOfArgumentsAfterOperator,
                                                   unsigned int tokenIndex) const;
    bool operatorParenthesisAreAlreadyPlaced(const TokenString &tokenString,
                                             const unsigned int leftParenthesisIndex,
                                             const unsigned int rightParenthesisIndex) const;
    void insertOperatorParenthesis(TokenString &tokenString,
                                   const unsigned int rightParenthesisInsertIndex,
                                   const unsigned int leftParenthesisInsertIndex) const;
    void moveOperator(TokenString &tokenString,
                      const unsigned int leftParenthesisInsertIndex,
                      const unsigned int tokenIndex) const;
    void processToken(TokenString &tokenString, const unsigned int tokenIndex, const EnhancedRecord &record) const;
};

#endif // BASICPREPROCESSOR_H
