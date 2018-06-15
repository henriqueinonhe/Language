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
