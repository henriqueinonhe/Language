#include "formatter.h"

Formatter::Formatter()
{

}

QString Formatter::format(QString string) const
{
    std::for_each(processors.begin(), processors.end(), [&string](const ProcessorEntry processor)
    {
        processor.processString(string);
    });

    return string;
}

void Formatter::addProcessor(StringProcessor *processor)
{
    processors.push_back(ProcessorEntry(processor));
}

void Formatter::removeProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors.remove(index);
}

void Formatter::moveProcessor(const unsigned int locationIndex, const unsigned int targetIndex)
{
    checkIndexIsWithinBounds(locationIndex);
    checkIndexIsWithinBounds(targetIndex);

    ProcessorEntry tempLocationProcessor = processors[locationIndex];
    processors.remove(locationIndex);
    processors.insert(targetIndex, tempLocationProcessor);
}

void Formatter::turnOnProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors[index].turnOn();
}

void Formatter::turnOffProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors[index].turnOff();
}

void Formatter::toggleProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors[index].toggle();
}

QString Formatter::toString() const
{
    QString string;

    std::for_each(processors.begin(), processors.end(), [&string](const ProcessorEntry &entry)
    {
        string += entry.toString();
    });

    return string;
}

void Formatter::checkIndexIsWithinBounds(const unsigned int index)
{
    if(index >= static_cast<unsigned int>(processors.size()))
    {
        throw std::invalid_argument("Processor index is out of bounds!");
    }
}
