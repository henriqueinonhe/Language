#ifndef BINDINGRECORD_H
#define BINDINGRECORD_H

#include <QVector>

class QDataStream;

struct BindingRecord final
{
public:
    BindingRecord() = default;
    BindingRecord(const BindingRecord &) = default;
    BindingRecord(BindingRecord &&) noexcept = default;
    BindingRecord &operator =(const BindingRecord &) = default;
    BindingRecord &operator =(BindingRecord &&) noexcept = default;
    ~BindingRecord() = default;

    BindingRecord(QDataStream &stream);
    BindingRecord(const unsigned int bindingArgumentIndex,
                  const QVector<unsigned int> &boundArgumentsIndexes);

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
