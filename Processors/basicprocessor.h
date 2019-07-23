#ifndef BASICPROCESSOR_H
#define BASICPROCESSOR_H

#include "stringprocessor.h"
#include "basicprocessortokenrecord.h"

class Signature;

class BasicProcessor : public StringProcessor
{
public:
    BasicProcessor() = delete;
    BasicProcessor(const BasicProcessor &) = delete;
    BasicProcessor(BasicProcessor &&) = delete;
    BasicProcessor &operator =(const BasicProcessor &) = delete;
    BasicProcessor &operator =(BasicProcessor &&) = delete;

    BasicProcessor(const Signature * const signature);

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

    virtual QString processString(const QString &string) const override = 0;

    QString toString() const override = 0;

    virtual ~BasicProcessor() = default;

protected:
    void serialize(QDataStream &stream) const override;
    void deserialize(QDataStream &stream) override;

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

    using TokenStringWrapper = QLinkedList<TokenWrapper>;
    using TokenStringWrapperIterator = QLinkedList<TokenWrapper>::iterator ;
    using TokenStringWrapperReverseIterator = QLinkedList<TokenWrapper>::reverse_iterator;

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

private:
    void checkExistsConflictingTokenRecord(const QString &token);
};

#endif // BASICPROCESSOR_H
