#ifndef FORMATTER_H
#define FORMATTER_H

#include <QString>
#include <QVector>
#include "stringprocessor.h"
#include "tokenstring.h"

class Formatter
{
public:
    Formatter();
    QString format(QString string);

private:
    class ProcessorEntry
    {
    public:
        ProcessorEntry(StringProcessor *processor) :
            processor(processor),
            isOnline(true)
        {
        }

        QString processString(QString string)
        {
            if(isOnline)
            {
                processor->processString(string);
            }
        }

    private:
        StringProcessor *processor;
        bool isOnline;
    };

    QVector<ProcessorEntry> processors;
};

#endif // FORMATTER_H
