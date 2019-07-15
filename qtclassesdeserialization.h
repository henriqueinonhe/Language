#ifndef QTCLASSESDESERIALIZATION_H
#define QTCLASSESDESERIALIZATION_H

/* The purpose of this class is to
 * implement Qt classes deserialization
 * via initialization, that is, via a "Copy Constructor"
 * instead of a "Copy Assignment" */

#include <QString>
#include <QStringList>
#include <QVector>
#include <QLinkedList>
#include <QDataStream>

class QDataStream;

namespace QtDeserialization {

    QString deserializeQString(QDataStream &stream);
    QStringList deserializeQStringList(QDataStream &stream);

    template <class T>
    QVector<T> deserializeQVector(QDataStream &stream)
    {
        QVector<T> vec;
        stream >> vec;
        return vec;
    }

    template <class T>
    QLinkedList<T> deserializeQLinkedList(QDataStream &stream)
    {
        QLinkedList<T> list;
        stream >> list;
        return list;
    }
}

#endif // QTCLASSESDESERIALIZATION_H
