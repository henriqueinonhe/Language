#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>
#include <QVector>
#include "stringprocessor.h"
#include <QDataStream>

class QDataStream;
class StringProcessor;

class Formatter
{
public:
    Formatter();
    Formatter(QDataStream &stream, const QVector<StringProcessor *> &processors);
    Formatter(const Formatter&) = delete;

    Formatter &operator=(const Formatter &) = delete;
    void deserialize(QDataStream &stream, const QVector<StringProcessor *> &processors);

    QString format(QString string) const;

    void addProcessor(StringProcessor *processor);
    void removeProcessor(const unsigned int index);
    void moveProcessor(const unsigned int locationIndex, const unsigned int targetIndex);

    void turnOnProcessor(const unsigned int index);
    void turnOffProcessor(const unsigned int index);
    void toggleProcessor(const unsigned int index);

    QString toString() const;

    ~Formatter() = default;

private:
    class ProcessorEntry
    {
    public:
        ProcessorEntry(){}

        ProcessorEntry(QDataStream &stream, StringProcessor *processor) :
            processor(processor)
        {
            stream >> isOnline;
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

    void checkIndexIsWithinBounds(const unsigned int index);

    QVector<ProcessorEntry> processors;

    friend QDataStream &operator <<(QDataStream &stream, const Formatter &formatter);
    friend QDataStream &operator <<(QDataStream &stream, const ProcessorEntry &entry);
};

QDataStream &operator <<(QDataStream &stream, const Formatter &formatter);
QDataStream &operator <<(QDataStream &stream, const Formatter::ProcessorEntry &entry);

#endif // FORMATTER_H
