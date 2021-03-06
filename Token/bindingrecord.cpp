#include "bindingrecord.h"
#include "containerauxiliarytools.h"
#include <QDataStream>
#include "qtclassesdeserialization.h"

BindingRecord::BindingRecord(QDataStream &stream) :
    bindingArgumentIndex(QtDeserialization::deserializeUInt(stream)),
    boundArgumentsIndexes(QtDeserialization::deserializeQVector<unsigned int>(stream))
{
}

void BindingRecord::checkBoundArgumentIndexesDuplicates(const QVector<unsigned int> &boundArgumentsIndexes)
{
    if(ContainerAuxiliaryTools::checkForDuplicates(boundArgumentsIndexes))
    {
        throw std::invalid_argument("There are duplicates in the bound argument indexes vector!");
    }
}

QDataStream &operator <<(QDataStream &stream, const BindingRecord &record)
{
    stream << record.bindingArgumentIndex <<  record.boundArgumentsIndexes;

    return stream;
}

QDataStream &operator >>(QDataStream &stream, BindingRecord &record)
{
    stream >> record.bindingArgumentIndex >> record.boundArgumentsIndexes;

    return stream;
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
