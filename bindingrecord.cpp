#include "bindingrecord.h"

BindingRecord::BindingRecord()
{

}

BindingRecord::BindingRecord(const unsigned int bindingArgumentIndex, const QVector<unsigned int> &boundArgumentsIndexes) :
    bindingArgumentIndex(bindingArgumentIndex),
    boundArgumentsIndexes(boundArgumentsIndexes)
{
    if(ContainerAuxiliaryTools<QVector<unsigned int>>::checkForDuplicates(boundArgumentsIndexes))
    {
        throw std::invalid_argument(""); //FIXME sdas
    }
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
