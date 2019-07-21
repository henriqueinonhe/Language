#include "formatter.h"
#include <QDataStream>

Formatter::Formatter()
{

}

Formatter::Formatter(QDataStream &stream, const QVector<StringProcessor *> &processors) :
    processors(deserializeEntries(stream, processors))
{

}

void Formatter::deserialize(QDataStream &stream, const QVector<StringProcessor *> &processors)
{
    int size;
    stream >> size;
    QVector<ProcessorEntry> entries;
    for(auto index = 0; index < size; index++)
    {
        entries.push_back(ProcessorEntry(stream, processors[index]));
    }
    this->processors = entries;
}

QString Formatter::format(QString string) const
{
    for(const auto &processor : processors)
    {
        string = processor.processString(string);
    }

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
    //FIXME
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
    for(const auto &entry : processors)
    {
        string += entry.toString();
    }

    return string;
}

QVector<Formatter::ProcessorEntry> Formatter::deserializeEntries(QDataStream &stream, const QVector<StringProcessor *> &processors)
{
    QVector<Formatter::ProcessorEntry> entries;
    int size;
    stream >> size;
    for(auto index = 0; index < size; index++)
    {
        entries.push_back(Formatter::ProcessorEntry(stream, processors[index]));
    }
    return entries;
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
    stream << formatter.processors;
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const Formatter::ProcessorEntry &entry)
{
    stream << entry.isOnline;
    return stream;
}
