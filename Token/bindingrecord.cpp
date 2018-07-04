#include "bindingrecord.h"

BindingRecord::BindingRecord()
{

}

void BindingRecord::checkBoundArgumentIndexesDuplicates(const QVector<unsigned int> &boundArgumentsIndexes)
{
    if(ContainerAuxiliaryTools<QVector<unsigned int>>::checkForDuplicates(boundArgumentsIndexes))
    {
        throw std::invalid_argument("There are duplicates in the bound argument indexes vector!");
    }
}

BindingRecord::BindingRecord(const unsigned int bindingArgumentIndex, const QVector<unsigned int> &boundArgumentsIndexes) :
    bindingArgumentIndex(bindingArgumentIndex),
    boundArgumentsIndexes(boundArgumentsIndexes)
{
    checkBoundArgumentIndexesDuplicates(boundArgumentsIndexes);
}

bool BindingRecord::operator==(const BindingRecord &other) const
{
    return this->bindingArgumentIndex == other.bindingArgumentIndex &&
            this->boundArgumentsIndexes == other.boundArgumentsIndexes;
}

bool BindingRecord::operator!=(const BindingRecord &other) const
{
    return !(*this == other);
}
