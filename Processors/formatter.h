#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>
#include <QVector>
#include "stringprocessor.h"
#include <QDataStream>
#include "qtclassesdeserialization.h"

class QDataStream;
class StringProcessor;

class Formatter
{
public:
    Formatter();
    Formatter(const Formatter&) = default;
    Formatter(Formatter &&) noexcept = default;
    Formatter &operator=(const Formatter &) = default;
    Formatter &operator=(Formatter &&) noexcept = default;
    ~Formatter() noexcept = default;

    Formatter(QDataStream &stream, const QVector<StringProcessor *> &processorsEntries);

    void deserialize(QDataStream &stream, const QVector<StringProcessor *> &processorsEntries);

    QString format(QString string) const;

    void addProcessor(StringProcessor *processor);
    void removeProcessor(const unsigned int index);
    void moveProcessor(const unsigned int locationIndex, const unsigned int targetIndex);

    void turnOnProcessor(const unsigned int index);
    void turnOffProcessor(const unsigned int index);
    void toggleProcessor(const unsigned int index);

    QStringList toStringList() const;

private:
    class ProcessorEntry
    {
    public:
        ProcessorEntry(){}

        ProcessorEntry(QDataStream &stream, StringProcessor *processor) :
            processor(processor),
            isOnline(QtDeserialization::deserializeBool(stream))
        {
        }

        ProcessorEntry(StringProcessor *processor) :
            processor(processor),
            isOnline(true)
        {
        }

        QString processString(const QString &string) const
        {
            if(isOnline)
            {
                return processor->processString(string);
            }
            else
            {
                return string;
            }
        }

        void turnOn()
        {
            isOnline = true;
        }

        void turnOff()
        {
            isOnline = false;
        }

        void toggle()
        {
            isOnline = !isOnline;
        }

        QString toString() const
        {
            QString string;

            string += "Processor: ";
            string += processor->toString();
            string += "\n";

            string += "Status: ";
            string += isOnline ? "Online" : "Offline";

            return string;
        }

    private:
        StringProcessor *processor;
        bool isOnline;

        friend QDataStream &operator <<(QDataStream &stream, const ProcessorEntry &entry);
    };

    QVector<ProcessorEntry> deserializeEntries(QDataStream &stream, const QVector<StringProcessor *> &processorsEntries);
    void checkIndexIsWithinBounds(const unsigned int index);

    QVector<ProcessorEntry> processorsEntries;

    friend QDataStream &operator <<(QDataStream &stream, const Formatter &formatter);
    friend QDataStream &operator <<(QDataStream &stream, const ProcessorEntry &entry);
};

QDataStream &operator <<(QDataStream &stream, const Formatter &formatter);
QDataStream &operator <<(QDataStream &stream, const Formatter::ProcessorEntry &entry);

#endif // FORMATTER_H
