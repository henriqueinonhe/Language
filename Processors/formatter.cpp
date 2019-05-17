#include "formatter.h"
#include <QDataStream>

Formatter::Formatter()
{

}

QString Formatter::format(QString string) const
{
    std::for_each(processors.begin(), processors.end(), [&string](const ProcessorEntry processor)
    {
        string = processor.processString(string);
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

    processors.remove(static_cast<int>(index));
}

void Formatter::moveProcessor(const unsigned int locationIndex, const unsigned int targetIndex)
{
    checkIndexIsWithinBounds(locationIndex);
    checkIndexIsWithinBounds(targetIndex);

    ProcessorEntry tempLocationProcessor = processors[static_cast<int>(locationIndex)];
    processors.remove(static_cast<int>(locationIndex));
    processors.insert(static_cast<int>(targetIndex), tempLocationProcessor);
}

void Formatter::turnOnProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors[static_cast<int>(index)].turnOn();
}

void Formatter::turnOffProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors[static_cast<int>(index)].turnOff();
}

void Formatter::toggleProcessor(const unsigned int index)
{
    checkIndexIsWithinBounds(index);

    processors[static_cast<int>(index)].toggle();
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

QDataStream &operator <<(QDataStream &stream, const Formatter &formatter)
{
    //TODO

    return stream;
}

QDataStream &operator >>(QDataStream &stream, Formatter &formatter)
{
    //TODO

    return stream;
}
