#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>
#include <QVector>
#include "stringprocessor.h"

class QDataStream;
class StringProcessor;

class Formatter
{
public:
    Formatter();

    QString format(QString string) const;

    void addProcessor(StringProcessor *processor);
    void removeProcessor(const unsigned int index);
    void moveProcessor(const unsigned int locationIndex, const unsigned int targetIndex);

    void turnOnProcessor(const unsigned int index);
    void turnOffProcessor(const unsigned int index);
    void toggleProcessor(const unsigned int index);

    QString toString() const;

private:
    class ProcessorEntry
    {
    public:
        ProcessorEntry(){}

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
    };

    void checkIndexIsWithinBounds(const unsigned int index);

    QVector<ProcessorEntry> processors;

    friend QDataStream &operator <<(QDataStream &stream, const Formatter &formatter);
    friend QDataStream &operator >>(QDataStream &stream, Formatter &formatter);
};

QDataStream &operator <<(QDataStream &stream, const Formatter &formatter);
QDataStream &operator >>(QDataStream &stream, Formatter &formatter);

#endif // FORMATTER_H
