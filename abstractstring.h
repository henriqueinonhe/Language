#ifndef ABSTRACTSTRING_H
#define ABSTRACTSTRING_H

#include <QString>

template <class T>

class AbstractString
{
    AbstractString(){}

    virtual QString toString() const = 0;

    unsigned int size() const;

    T operator[](unsigned int index) const;
    T first() const;
    T last() const;

    bool isLastIndex(const unsigned int index) const;
    bool indexIsWithinBounds(const unsigned int index) const;
    bool isEmpty() const;

    bool operator==(const TypeTokenString &other) const;
    bool operator!=(const TypeTokenString &other) const;
};

#endif // ABSTRACTSTRING_H
