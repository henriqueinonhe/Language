#ifndef POOL_H
#define POOL_H

#include <QVector>
#include <QLinkedList>

template<class T>
class PoolRecordPointer;
template<class T>
class PoolRecord;

template<class T>
class Pool
{
public:
    Pool()
    {
    }

    PoolRecordPointer<T> getPointer(const T &sample)
    {
        for(auto iter = records.begin(); iter != records.end(); iter++)
        {
            if(iter->getObject() == sample)
            {
                return PoolRecordPointer<T>(&(*iter));
            }
        }

        records.push_back(PoolRecord<T>(sample, this));

        return PoolRecordPointer<T>(&records.back());
    }

    QLinkedList<PoolRecord<T> > &getRecords()
    {
        return records;
    }

private:
    QLinkedList<PoolRecord<T>> records;

template<class T2>
friend class PoolRecordPointer;

};

template<class T>
class PoolRecord
{
public:
    PoolRecord()
    {}

    T &getObject()
    {
        return object;
    }

    unsigned int getCounter() const
    {
        return counter;
    }

    bool operator==(const PoolRecord &other) const
    {
        return this->object == other.object &&
               this->parent == other.parent;
    }

private:

    PoolRecord(const T &object, Pool<T> *parent) :
        object(object),
        counter(0),
        parent(parent)
    {
    }

    void incrementCounter()
    {
        counter++;
    }

    void decrementCounter()
    {
        counter--;
    }

    bool counterIsZero() const
    {
        return counter == 0;
    }

    T object;
    unsigned int counter;
    Pool<T> *parent;

template<class T2>
friend class Pool;

template<class T3>
friend class PoolRecordPointer;

};


template <class T>
class PoolRecordPointer
{
public:
    PoolRecordPointer() :
        isSet(false)
    {}

    PoolRecordPointer(PoolRecord<T> *ptr) :
        ptr(ptr),
        isSet(true)
    {
        ptr->incrementCounter();
    }

    PoolRecordPointer(const PoolRecordPointer &other) :
        ptr(other.ptr),
        isSet(other.isSet)
    {
        ptr->incrementCounter();
    }

    PoolRecordPointer &operator=(const PoolRecordPointer &other)
    {
        recountPointer();

        ptr = other.ptr;
        isSet = other.isSet;
        ptr->incrementCounter();

        return *this;
    }

    T operator*() const
    {
        return ptr->getObject();
    }

    T *operator->() const
    {
        return &ptr->getObject();
    }


    ~PoolRecordPointer()
    {
        recountPointer();
    }

private:
    void recountPointer()
    {
        if(isSet)
        {
            ptr->decrementCounter();

            if(ptr->counterIsZero())
            {
                ptr->parent->records.removeOne(*ptr);
                ptr = nullptr;
                isSet = false;
            }
        }
    }

    PoolRecord<T> *ptr;
    bool isSet;
};

#endif // POOL_H
