#include "qtclassesdeserialization.h"

#include <QDataStream>

QString QtDeserialization::deserializeQString(QDataStream &stream)
{
    QString str;
    stream >> str;
    return str;
}

QStringList QtDeserialization::deserializeQStringList(QDataStream &stream)
{
    QStringList list;
    stream >> list;
    return list;
}
