#include "bindingtoken.h"

BindingToken::BindingToken(const QString &token, const Type &type, const QVector<BindingToken::BindingRecord> &bindingRecords) :
    CoreToken(token, type)
{
    validateBindingRecords(bindingRecords);
    this->bindingRecords = bindingRecords;
}

void BindingToken::validateBindingRecords(const QVector<BindingRecord> &bindingRecords) const
{
    //FIXME Refactor this, FFS

    if(ContainerAuxiliaryTools<QVector<BindingRecord>>::checkForDuplicates(bindingRecords))
    {
        throw;
    }


    QVector<unsigned int> bindingArgumentsIndexes;

    std::for_each(bindingRecords.begin(), bindingRecords.end(), [&bindingArgumentsIndexes](const BindingRecord &record)
    {
        bindingArgumentsIndexes.push_back(record.bindingArgumentIndex);
    });

    if(ContainerAuxiliaryTools<QVector<unsigned int>>::checkForDuplicates(bindingArgumentsIndexes))
    {
        throw;
    }

    QVector<unsigned int> boundArgumentsIndexes;

    std::for_each(bindingRecords.begin(), bindingRecords.end(), [&boundArgumentsIndexes](const BindingRecord &record)
    {
        std::for_each(record.boundArgumentsIndexes.begin(), record.boundArgumentsIndexes.end(), [&boundArgumentsIndexes](const unsigned int index)
        {
            boundArgumentsIndexes.push_back(index);
        });
    });

    //Check for intersection of records
    if(!ContainerAuxiliaryTools<QVector<unsigned int>>::containersAreDisjoint(bindingArgumentsIndexes, boundArgumentsIndexes))
    {
        throw; //TODO
    }

    const unsigned int greatestBindingIndex = *std::max_element(bindingArgumentsIndexes.begin(), bindingArgumentsIndexes.end());
    const unsigned int greatestBoundIndex = *std::max_element(boundArgumentsIndexes.begin(), boundArgumentsIndexes.end());
    const unsigned int numberOfArguments = type.getArgumentsTypes().size();

    if(greatestBindingIndex > numberOfArguments || greatestBoundIndex > numberOfArguments)
    {
        throw;
    }

    //Now I think that this covers the whole process of validation!
}
