#ifndef QTCLASSESDESERIALIZATION_H
#define QTCLASSESDESERIALIZATION_H

/* The purpose of this class is to
 * implement Qt classes deserialization
 * via initialization, that is, via a "Copy Constructor"
 * instead of a "Copy Assignment" */

#include <QDataStream>
#include <memory>
#include <QVector>
#include <QLinkedList>

class QDataStream;

namespace QtDeserialization {

    int deserializeInt(QDataStream &stream);
    unsigned int deserializeUInt(QDataStream &stream);
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

    template <class T, class ... Args>
    QVector<std::shared_ptr<T>> deserializeSharedPointerQVector(QDataStream &stream, Args ... args)
    {
        //NOTE Not quite sure this is right!
        int size;
        stream >> size;
        QVector<std::shared_ptr<T>> vec;
        for(auto i = 0; i < size; i++)
        {
            vec.push_back(std::make_shared<T>(stream, args ...));
        }
        return vec;
    }
}

#endif // QTCLASSESDESERIALIZATION_H
