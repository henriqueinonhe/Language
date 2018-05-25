#ifndef BASICPREPROCESSOR_H
#define BASICPREPROCESSOR_H

#include "basicprocessor.h"

class BasicPreProcessor : public BasicProcessor
{
public:
    BasicPreProcessor(Signature * const signature);

    QString processString(QString string) const;

    QString toString() const;

private:
    bool tokenAlreadyConsidered(const Token &token, const QVector<EnhancedRecord> &necessaryRecords) const;

    void considerToken(const Token &currentToken, QVector<EnhancedRecord> &necessaryRecords) const;
};

#endif // BASICPREPROCESSOR_H
