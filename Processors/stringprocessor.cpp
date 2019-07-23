#include "stringprocessor.h"
#include <QDataStream>


QDataStream &operator <<(QDataStream &stream, const StringProcessor &processor)
{
    processor.serialize(stream);
    return stream;
}

QDataStream &operator >>(QDataStream &stream, StringProcessor &processor)
{
    processor.deserialize(stream);
    return stream;
}

StringProcessor::StringProcessor(const Signature * const signature) :
    signature(signature)
{

}

StringProcessor::~StringProcessor()
{

}
