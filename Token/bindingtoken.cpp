#include "bindingtoken.h"

BindingToken::BindingToken(const QString &token, const Type &type, const QVector<BindingRecord> &bindingRecords) :
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

QVector<BindingRecord> BindingToken::getBindingRecords() const
{
    return bindingRecords;
}

QString BindingToken::tokenClass() const
{
    return "BindingToken";
}

Token *BindingToken::getAllocatedClone() const
{
    return new BindingToken(*this);
}

bool BindingToken::isEqual(const Token &other) const
{
    return CoreToken::isEqual(other) &&
           this->bindingRecords == dynamic_cast<const BindingToken &>(other).bindingRecords;
}

void BindingToken::checkEmptyRecords(const QVector<BindingRecord> &bindingRecords) const
{
    if(bindingRecords.isEmpty())
    {
        throw std::invalid_argument("The Binding Records cannot be empty!");
    }
}

void BindingToken::checkDuplicateBindingRecords(const QVector<BindingRecord> &bindingRecords) const
{
    if(ContainerAuxiliaryTools<QVector<BindingRecord>>::checkForDuplicates(bindingRecords))
    {
        throw std::invalid_argument("There are duplicates in the binding records!");
    }
}

unsigned int BindingToken::getGreatestBindingArgumentNumber(const QVector<unsigned int> &bindingArgumentsIndexes) const
{
    const unsigned int zeroIndexCompensation = 1;
    const unsigned int greatestBindingNumber = *std::max_element(bindingArgumentsIndexes.begin(), bindingArgumentsIndexes.end()) + zeroIndexCompensation;

    return greatestBindingNumber;
}

unsigned int BindingToken::getGreatestBoundArgumentNumber(const QVector<unsigned int> &boundArgumentsIndexes) const
{
    const unsigned int zeroIndexCompensation = 1;
    const unsigned int greatestBoundNumber = *std::max_element(boundArgumentsIndexes.begin(), boundArgumentsIndexes.end()) + zeroIndexCompensation;

    return greatestBoundNumber;
}

void BindingToken::checkDuplicatesBindingArgumentsIndexes(const QVector<unsigned int> &bindingArgumentsIndexes) const
{
    if(ContainerAuxiliaryTools<QVector<unsigned int>>::checkForDuplicates(bindingArgumentsIndexes))
    {
        throw std::invalid_argument("There are at least two different binding records with the same binding index!");
    }
}

void BindingToken::checkArgumentIsBothBindingAndBound(const QVector<unsigned int> &boundArgumentsIndexes, const QVector<unsigned int> &bindingArgumentsIndexes) const
{
    if(!ContainerAuxiliaryTools<QVector<unsigned int>>::containersAreDisjoint(bindingArgumentsIndexes, boundArgumentsIndexes))
    {
        throw std::invalid_argument("There is at least one index in the binding records that is assigned simultaneously to a binding index and a bound index!");
    }
}

void BindingToken::checkNumberOfArgumentsConsistency(const unsigned int greatestBindingArgumentNumber, const unsigned int greatestBoundArgumentNumber, const unsigned int numberOfArguments) const
{
    if(greatestBindingArgumentNumber > numberOfArguments || greatestBoundArgumentNumber > numberOfArguments)
    {
        throw std::invalid_argument("The number of arguments required by the binding records is inconsistent with the number of arguments of the token's type you provided.");
    }
}

void BindingToken::validateBindingRecordsArguments(const QVector<BindingRecord> &bindingRecords) const
{
    const QVector<unsigned int> bindingArgumentsIndexes = gatherBindingArgumentsIndexes(bindingRecords);
    checkDuplicatesBindingArgumentsIndexes(bindingArgumentsIndexes);

    const QVector<unsigned int> boundArgumentsIndexes = gatherBoundArgumentsIndexes(bindingRecords);
    checkArgumentIsBothBindingAndBound(boundArgumentsIndexes, bindingArgumentsIndexes);

    const unsigned int greatestBindingArgumentNumber = getGreatestBindingArgumentNumber(bindingArgumentsIndexes);
    const unsigned int greatestBoundArgumentNumber = getGreatestBoundArgumentNumber(boundArgumentsIndexes);
    const unsigned int numberOfArguments = type.getArgumentsTypes().size();

    checkNumberOfArgumentsConsistency(greatestBindingArgumentNumber, greatestBoundArgumentNumber, numberOfArguments);
}

void BindingToken::validateBindingRecords(const QVector<BindingRecord> &bindingRecords) const
{
    checkEmptyRecords(bindingRecords);
    checkDuplicateBindingRecords(bindingRecords);
    validateBindingRecordsArguments(bindingRecords);
}
