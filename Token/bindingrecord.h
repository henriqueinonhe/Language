#ifndef BINDINGRECORD_H
#define BINDINGRECORD_H

#include "containerauxiliarytools.h"
#include <QVector>

struct BindingRecord
{
public:
    BindingRecord();

    BindingRecord(const unsigned int bindingArgumentIndex,
                  const QVector<unsigned int> &boundArgumentsIndexes);

    bool operator==(const BindingRecord &other) const;
    bool operator!=(const BindingRecord &other) const;

    unsigned int bindingArgumentIndex;
    QVector<unsigned int> boundArgumentsIndexes;
private:
    void checkBoundArgumentIndexesDuplicates(const QVector<unsigned int> &boundArgumentsIndexes);
};

#endif // BINDINGRECORD_H
