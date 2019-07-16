#ifndef BINDINGTOKEN_H
#define BINDINGTOKEN_H

#include "coretoken.h"
#include "bindingrecord.h"

class BindingToken : public CoreToken
{
public:
    BindingToken(QDataStream &stream);
    BindingToken(const QString &token,
                 const Type &type,
                 const QVector<BindingRecord> &bindingRecords);

    QVector<BindingRecord> getBindingRecords() const;

    virtual QString tokenClass() const override;

    virtual Token *getAllocatedClone() const override;


protected:
    virtual void serialize(QDataStream &stream) const override;

    virtual bool isEqual(const Token &other) const override;

private:

    void validateBindingRecords(const QVector<BindingRecord> &bindingRecords) const;

    QVector<unsigned int> gatherBindingArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const;
    QVector<unsigned int> gatherBoundArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const;

    void checkEmptyRecords(const QVector<BindingRecord> &bindingRecords) const;
    void checkDuplicateBindingRecords(const QVector<BindingRecord> &bindingRecords) const;
    void validateBindingRecordsArguments(const QVector<BindingRecord> &bindingRecords) const;
    unsigned int getGreatestBindingArgumentNumber(const QVector<unsigned int> &bindingArgumentsIndexes) const;
    unsigned int getGreatestBoundArgumentNumber(const QVector<unsigned int> &boundArgumentsIndexes) const;
    void checkDuplicatesBindingArgumentsIndexes(const QVector<unsigned int> &bindingArgumentsIndexes) const;
    void checkArgumentIsBothBindingAndBound(const QVector<unsigned int> &boundArgumentsIndexes, const QVector<unsigned int> &bindingArgumentsIndexes) const;
    void checkNumberOfArgumentsConsistency(const unsigned int greatestBindingArgumentNumber, const unsigned int greatestBoundArgumentNumber, const unsigned int numberOfArguments) const;

    QVector<BindingRecord> bindingRecords;

    friend Token *Token::deserializePtr(QDataStream &stream);
};

#endif // BINDINGTOKEN_H
