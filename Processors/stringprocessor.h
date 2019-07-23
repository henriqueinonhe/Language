#ifndef STRINGPROCESSOR_H
#define STRINGPROCESSOR_H

class QString;
class QDataStream;
class Signature;

class StringProcessor
{
public:
    StringProcessor(const Signature * const signature);
    StringProcessor(QDataStream &stream, Signature * const signature);

    virtual QString processString(const QString &string) const = 0;

    virtual QString toString() const = 0;

    virtual ~StringProcessor();

protected:
    virtual void serialize(QDataStream &stream) const = 0;
    virtual void deserialize(QDataStream &stream) = 0;

    const Signature *signature;

friend QDataStream &operator <<(QDataStream &stream, const StringProcessor &processor);
friend QDataStream &operator >>(QDataStream &stream, StringProcessor &processor);
};

#endif // STRINGPROCESSOR_H

QDataStream &operator <<(QDataStream &stream, const StringProcessor &processor);
QDataStream &operator >>(QDataStream &stream, StringProcessor &processor);
