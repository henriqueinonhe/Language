#ifndef BASICPROCESSOR_H
#define BASICPROCESSOR_H

#include "stringprocessor.h"
#include "basicprocessortokenrecord.h"
#include "lexer.h"

class BasicProcessor : virtual public StringProcessor
{
public:
    BasicProcessor(Signature * const signature);

    void addTokenRecord(CoreToken * const token,
                        const unsigned int position,
                        const BasicProcessorTokenRecord::Associativity associativity,
                        const unsigned int precedenceRank);
    void removeTokenRecord(const QString &tokenString);

    unsigned int getPrecendeRank(const QString &tokenString) const;

    QString processString(QString string) const = 0;

    QString toString() const = 0;

private:
    QLinkedList<BasicProcessorTokenRecord> tokenRecords;
    Lexer lexer;
};

#endif // BASICPROCESSOR_H
