#ifndef BASICPROCESSOR_H
#define BASICPROCESSOR_H

#include "stringprocessor.h"
#include "basicprocessortokenrecord.h"
#include "lexer.h"

class BasicProcessor : virtual public StringProcessor
{
public:
    BasicProcessor(Signature * const signature);

    void addTokenRecord(const QString &token,
                        const unsigned int position,
                        const int precedenceRank = -1,
                        const BasicProcessorTokenRecord::Associativity associativity = BasicProcessorTokenRecord::Associativity::Left);
    void insertTokenRecord(const QString &token,
                        const unsigned int position,
                        const int precedenceRank = -1,
                        const BasicProcessorTokenRecord::Associativity associativity = BasicProcessorTokenRecord::Associativity::Left); //FIXME
    void removeTokenRecord(const QString &tokenString);

    unsigned int getOperatorPrecedenceRank(const QString &tokenString) const;
    unsigned int getOperatorPosition(const QString &tokenString) const;
    BasicProcessorTokenRecord::Associativity getOperatorAssociativity(const QString &tokenString) const;

    virtual QString processString(const QString &string) const = 0;

    QString toString() const = 0;

protected:
    struct TokenWrapper
    {
        TokenWrapper();
        TokenWrapper(const QString &token) :
            token(token),
            alreadyScanned(false)
        {}

        bool operator==(const TokenWrapper &other) const
        {
            return this->token == other.token;
        }

        QString token;
        bool alreadyScanned;
    };

    typedef QLinkedList<TokenWrapper> TokenStringWrapper;
    typedef QLinkedList<TokenWrapper>::iterator TokenStringWrapperIterator;
    typedef QLinkedList<TokenWrapper>::reverse_iterator TokenStringWrapperReverseIterator;

    const BasicProcessorTokenRecord *getTokenRecordPtr(const QString &token) const;

    TokenStringWrapper wrapTokenString(const QString &string) const;
    TokenStringWrapperIterator findDelimiterScopeEndIterator(const TokenStringWrapper &tokenString,
                                       TokenStringWrapperIterator iter) const;
    void findDelimiterScopeEndReverseIterator(const TokenStringWrapper &tokenString,
                                              TokenStringWrapperReverseIterator &iter) const;
    TokenStringWrapperIterator findOperatorLeftParenthesisIterator(const TokenStringWrapper &tokenString,
                                                                   const unsigned int numberOfArgumentsBeforeOperator,
                                                                   const TokenStringWrapperIterator &tokenStringIter) const;
    TokenStringWrapperIterator findOperatorRightParenthesisIterator(const TokenStringWrapper &tokenString,
                                                                    const unsigned int numberOfArgumentsAfterOperator,
                                                                    const TokenStringWrapperIterator &tokenStringIter) const;

    QString tokenStringWrapperToString(TokenStringWrapper tokenString) const;
    bool tokenNeedsSubsequentSeparation(const TokenStringWrapper &tokenString, const TokenStringWrapperIterator &iter) const;

    QLinkedList<BasicProcessorTokenRecord> tokenRecords;
    Signature *signature;

private:
    void checkExistsConflictingTokenRecord(const QString &token);
};

#endif // BASICPROCESSOR_H
