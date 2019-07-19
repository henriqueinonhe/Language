#include "qtclassesdeserialization.h"

#include <QString>
#include <QDataStream>
#include <QStringList>

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

unsigned int QtDeserialization::deserializeUInt(QDataStream &stream)
{
    unsigned int val;
    stream >> val;
    return val;
}

int QtDeserialization::deserializeInt(QDataStream &stream)
{
    int val;
    stream >> val;
    return val;
}
