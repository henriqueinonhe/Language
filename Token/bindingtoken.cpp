#include "bindingtoken.h"
#include <QDataStream>
#include "containerauxiliarytools.h"

BindingToken::BindingToken(QDataStream &stream) :
    CoreToken(stream)
{
    stream >> bindingRecords;
}

BindingToken::BindingToken(const QString &token, const Type &type, const QVector<BindingRecord> &bindingRecords) :
    CoreToken(token, type)
{
    validateBindingRecords(bindingRecords);
    this->bindingRecords = bindingRecords;
}

QVector<unsigned int> BindingToken::gatherBindingArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const
{
    QVector<unsigned int> bindingArgumentsIndexes;
    for(const auto &record : bindingRecords)
    {
        bindingArgumentsIndexes.push_back(record.bindingArgumentIndex);
    }

    return bindingArgumentsIndexes;
}

QVector<unsigned int> BindingToken::gatherBoundArgumentsIndexes(const QVector<BindingRecord> &bindingRecords) const
{
    QVector<unsigned int> boundArgumentsIndexes;
    for(const auto &record : bindingRecords)
    {
        for(const auto index : record.boundArgumentsIndexes)
        {
            boundArgumentsIndexes.push_back(index);
        }
    }
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

void BindingToken::serialize(QDataStream &stream) const
{
    CoreToken::serialize(stream);
    stream << bindingRecords;
}

bool BindingToken::isEqual(const CoreToken &other) const
{
    return this->bindingRecords == static_cast<const BindingToken &>(other).bindingRecords &&
            isEqual(static_cast<const BindingToken &>(other));
}

bool BindingToken::isEqual(const BindingToken &other) const
{
    return true;
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
    if(ContainerAuxiliaryTools::checkForDuplicates(bindingRecords))
    {
        throw std::invalid_argument("There are duplicates in the binding records!");
    }
}

unsigned int BindingToken::getGreatestBindingArgumentNumber(const QVector<unsigned int> &bindingArgumentsIndexes) const
{
    const auto zeroIndexCompensation = 1;
    const auto greatestBindingNumber = *std::max_element(bindingArgumentsIndexes.begin(), bindingArgumentsIndexes.end()) + zeroIndexCompensation;

    return greatestBindingNumber;
}

unsigned int BindingToken::getGreatestBoundArgumentNumber(const QVector<unsigned int> &boundArgumentsIndexes) const
{
    const auto zeroIndexCompensation = 1;
    const auto greatestBoundNumber = *std::max_element(boundArgumentsIndexes.begin(), boundArgumentsIndexes.end()) + zeroIndexCompensation;

    return greatestBoundNumber;
}

void BindingToken::checkDuplicatesBindingArgumentsIndexes(const QVector<unsigned int> &bindingArgumentsIndexes) const
{
    if(ContainerAuxiliaryTools::checkForDuplicates(bindingArgumentsIndexes))
    {
        throw std::invalid_argument("There are at least two different binding records with the same binding index!");
    }
}

void BindingToken::checkArgumentIsBothBindingAndBound(const QVector<unsigned int> &boundArgumentsIndexes, const QVector<unsigned int> &bindingArgumentsIndexes) const
{
    if(!ContainerAuxiliaryTools::containersAreDisjoint(bindingArgumentsIndexes, boundArgumentsIndexes))
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
    const auto bindingArgumentsIndexes = gatherBindingArgumentsIndexes(bindingRecords);
    checkDuplicatesBindingArgumentsIndexes(bindingArgumentsIndexes);

    const auto boundArgumentsIndexes = gatherBoundArgumentsIndexes(bindingRecords);
    checkArgumentIsBothBindingAndBound(boundArgumentsIndexes, bindingArgumentsIndexes);

    const auto greatestBindingArgumentNumber = getGreatestBindingArgumentNumber(bindingArgumentsIndexes);
    const auto greatestBoundArgumentNumber = getGreatestBoundArgumentNumber(boundArgumentsIndexes);
    const auto numberOfArguments = static_cast<unsigned int>(getType().getArgumentsTypes().size());

    checkNumberOfArgumentsConsistency(greatestBindingArgumentNumber, greatestBoundArgumentNumber, numberOfArguments);
}

void BindingToken::validateBindingRecords(const QVector<BindingRecord> &bindingRecords) const
{
    checkEmptyRecords(bindingRecords);
    checkDuplicateBindingRecords(bindingRecords);
    validateBindingRecordsArguments(bindingRecords);
}
