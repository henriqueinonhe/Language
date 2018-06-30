#ifndef BINDINGTOKEN_H
#define BINDINGTOKEN_H

#include "coretoken.h"
#include "bindingrecord.h"

class BindingToken : public CoreToken
{
public:
    BindingToken(const QString &token,
                 const Type &type,
                 const QVector<BindingRecord> &bindingRecords);

    QVector<BindingRecord> getBindingRecords() const;

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;

protected:
    virtual bool isEqual(const Token &other) const;

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
};

#endif // BINDINGTOKEN_H
