#ifndef STRINGPROCESSOR_H
#define STRINGPROCESSOR_H

class QString;

class StringProcessor
{
public:
    virtual QString processString(const QString &string) const = 0;

    virtual QString toString() const = 0;

    virtual ~StringProcessor();
};

#endif // STRINGPROCESSOR_H

QDataStream &operator <<(QDataStream &stream, const StringProcessor &processor);
QDataStream &operator >>(QDataStream &stream, StringProcessor &processor);
