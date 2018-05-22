#include "bindingtoken.h"

BindingToken::BindingToken(const QString &token, const Type &type, const QVector<BindingToken::BindingRecord> &bindingRecords) :
    CoreToken(token, type)
{
    validateBindingRecords(bindingRecords);
    this->bindingRecords = bindingRecords;
}

QVector<unsigned int> BindingToken::gatherBindingArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const
{
    QVector<unsigned int> bindingArgumentsIndexes;
    std::for_each(bindingRecords.begin(), bindingRecords.end(), [&bindingArgumentsIndexes](const BindingRecord &record)
    {
        bindingArgumentsIndexes.push_back(record.bindingArgumentIndex);
    });

    return bindingArgumentsIndexes;
}

QVector<unsigned int> BindingToken::gatherBoundArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const
{
    QVector<unsigned int> boundArgumentsIndexes;
    std::for_each(bindingRecords.begin(), bindingRecords.end(), [&boundArgumentsIndexes](const BindingRecord &record)
    {
        std::for_each(record.boundArgumentsIndexes.begin(), record.boundArgumentsIndexes.end(), [&boundArgumentsIndexes](const unsigned int index)
        {
            boundArgumentsIndexes.push_back(index);
        });
    });

    return boundArgumentsIndexes;
}

QVector<BindingToken::BindingRecord> BindingToken::getBindingRecords() const
{
    return bindingRecords;
}

void BindingToken::validateBindingRecords(const QVector<BindingRecord> &bindingRecords) const
{
    //NOTE Maybe this can still be better refactored!
    if(bindingRecords.isEmpty())
    {
        throw std::invalid_argument("The Binding Records cannot be empty!");
    }

    if(ContainerAuxiliaryTools<QVector<BindingRecord>>::checkForDuplicates(bindingRecords))
    {
        throw std::invalid_argument("There are duplicates in the binding records!");
    }

    QVector<unsigned int> bindingArgumentsIndexes = gatherBindingArgumentsIndexes(bindingRecords);
    if(ContainerAuxiliaryTools<QVector<unsigned int>>::checkForDuplicates(bindingArgumentsIndexes))
    {
        throw std::invalid_argument("There are at least two different binding records with the same binding index.");
    }

    QVector<unsigned int> boundArgumentsIndexes = gatherBoundArgumentsIndexes(bindingRecords);

    //Check for intersection of records
    if(!ContainerAuxiliaryTools<QVector<unsigned int>>::containersAreDisjoint(bindingArgumentsIndexes, boundArgumentsIndexes))
    {
        throw std::invalid_argument("There is at least one index in the binding records that is assigned simultaneously to a binding index and a bound index!");
    }

    const unsigned int zeroIndexCompensation = 1;
    const unsigned int greatestBindingNumber = *std::max_element(bindingArgumentsIndexes.begin(), bindingArgumentsIndexes.end()) + zeroIndexCompensation;
    const unsigned int greatestBoundNumber = *std::max_element(boundArgumentsIndexes.begin(), boundArgumentsIndexes.end()) + zeroIndexCompensation;
    const unsigned int numberOfArguments = type.getArgumentsTypes().size();

    if(greatestBindingNumber > numberOfArguments || greatestBoundNumber > numberOfArguments)
    {
        throw std::invalid_argument("The number of arguments required by the binding records are inconsistent with the number of arguments of token's type you provided.");
    }
}
