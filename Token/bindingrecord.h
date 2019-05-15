#ifndef BINDINGRECORD_H
#define BINDINGRECORD_H

#include <QVector>

struct BindingRecord
{
public:
    BindingRecord();

    BindingRecord(const unsigned int bindingArgumentIndex,
                  const QVector<unsigned int> &boundArgumentsIndexes);

    BindingRecord(const QString &string);

    bool operator==(const BindingRecord &other) const;
    bool operator!=(const BindingRecord &other) const;

    unsigned int bindingArgumentIndex;
    QVector<unsigned int> boundArgumentsIndexes;

private:
    void checkBoundArgumentIndexesDuplicates(const QVector<unsigned int> &boundArgumentsIndexes);

    friend QDataStream &operator <<(QDataStream &stream, const BindingRecord &record);
    friend QDataStream &operator >>(QDataStream &stream, BindingRecord &record);
};

QDataStream &operator <<(QDataStream &stream, const BindingRecord &record);
QDataStream &operator >>(QDataStream &stream, BindingRecord &record);

#endif // BINDINGRECORD_H
