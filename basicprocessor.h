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
                        const BasicProcessorTokenRecord::Associativity associativity,
                        const int precedenceRank);
    void removeTokenRecord(const QString &tokenString);

    unsigned int getPrecedenceRank(const QString &tokenString) const;


    QString processString(QString string) const = 0;

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

    struct AuxiliaryTokenRecord
    {
        AuxiliaryTokenRecord(){}
        AuxiliaryTokenRecord(BasicProcessorTokenRecord * const record,
                       const unsigned int precedenceRank) :
            record(record),
            precedenceRank(precedenceRank)
        {}

        BasicProcessorTokenRecord *record;
        unsigned int precedenceRank;
        QVector<QLinkedList<TokenWrapper>::iterator> tokenIterators;
    };

    typedef QLinkedList<TokenWrapper> TokenStringWrapper;
    typedef QLinkedList<TokenWrapper>::iterator TokenStringWrapperIterator;
    typedef QLinkedList<TokenWrapper>::reverse_iterator TokenStringWrapperReverseIterator;

    const BasicProcessorTokenRecord *getTokenRecordPtr(const QString &token) const;

    QString tokenStringWrapperToString(const TokenStringWrapper &tokenString) const;

    QLinkedList<BasicProcessorTokenRecord> tokenRecords;
    Signature *signature;

};

#endif // BASICPROCESSOR_H
