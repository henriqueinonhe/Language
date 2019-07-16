#ifndef POOL_H
#define POOL_H

#include <QVector>
#include <QLinkedList>
#include <unordered_map>
#include <string>

//NOTE Maybe implement Serialization of this class!

using namespace std;

template<class T>
class Pool;

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
        const auto key = sample.getString();
        auto valueIter = records.find(key.toStdString());
        if(valueIter == records.end())
        {
            records.emplace(key.toStdString(), PoolRecord<T>(sample, this));
        }

        return PoolRecordPointer<T>(&records[key.toStdString()]);
    }

    unordered_map<string, PoolRecord<T>> &getRecords()
    {
        return records;
    }

private:
    unordered_map<string, PoolRecord<T>> records;

friend class PoolRecordPointer<T>;

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
        /* Only the pool may create pool records. */
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
    Pool<T> *parent; //Do we really need this?

    friend class Pool<T>;
    friend class PoolRecordPointer<T>;
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
                ptr->parent->records.erase(ptr->getObject().getString().toStdString());
                ptr = nullptr;
                isSet = false;
            }
        }
    }

    PoolRecord<T> *ptr;
    bool isSet; //To avoid using delete on a nullptr (removing a PoolRecord), when the PoolRecordPointer is still not set,
                //like while passing the pointer as value, or creating a new one in a vector.
};

#endif // POOL_H
