#ifndef BINDINGTOKEN_H
#define BINDINGTOKEN_H

#include "coretoken.h"
#include "containerauxiliarytools.h"

class BindingToken : public CoreToken
{
public:
    struct BindingRecord
    {
        BindingRecord(){}

        BindingRecord(const unsigned int bindingArgumentIndex,
                      const QVector<unsigned int> &boundArgumentsIndexes) :
            bindingArgumentIndex(bindingArgumentIndex),
            boundArgumentsIndexes(boundArgumentsIndexes)
        {
            if(ContainerAuxiliaryTools<QVector<unsigned int>>::checkForDuplicates(boundArgumentsIndexes))
            {
                throw std::invalid_argument("");
            }
        }

        bool operator==(const BindingRecord &other) const
        {
            return this->bindingArgumentIndex == other.bindingArgumentIndex &&
                   this->boundArgumentsIndexes == other.boundArgumentsIndexes;
        }

        bool operator!=(const BindingRecord &other) const
        {
            return !(*this == other);
        }

        unsigned int bindingArgumentIndex;
        QVector<unsigned int> boundArgumentsIndexes;
    };

    BindingToken(const QString &token,
                 const Type &type,
                 const QVector<BindingRecord> &bindingRecords);

    QVector<BindingRecord> getBindingRecords() const;

    virtual QString tokenClass() const;

private:
    void validateBindingRecords(const QVector<BindingRecord> &bindingRecords) const;

    QVector<unsigned int> gatherBindingArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const;
    QVector<unsigned int> gatherBoundArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const;

    QVector<BindingRecord> bindingRecords;
};

#endif // BINDINGTOKEN_H
