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

    unsigned int getPrecendeRank(const QString &tokenString) const;

    QString processString(QString string) const = 0;

    QString toString() const = 0;

protected:
    struct EnhancedRecord
    {
        EnhancedRecord(){}
        EnhancedRecord(BasicProcessorTokenRecord * const record,
                       const unsigned int precedenceRank) :
            record(record),
            precedenceRank(precedenceRank)
        {}

        BasicProcessorTokenRecord *record;
        unsigned int precedenceRank;
    };

    QLinkedList<BasicProcessorTokenRecord> tokenRecords;
    Signature *signature;
};

#endif // BASICPROCESSOR_H
