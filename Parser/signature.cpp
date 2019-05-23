#include "signature.h"

#include <QDataStream>

QDataStream &operator <<(QDataStream &stream, const Signature &signature)
{
    signature.serialize(stream);

    return stream;
}

QDataStream &operator >>(QDataStream &stream, Signature &signature)
{
    signature.unserialize(stream);

    return stream;
}


Signature::~Signature()
{

}
