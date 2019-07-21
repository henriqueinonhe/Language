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
    Pool() = default;
    Pool(const Pool &) = delete; //Could be implemented later
    Pool(Pool &&) = delete;
    Pool &operator =(const Pool &) = delete;
    Pool &operator =(Pool &&) = delete;
    ~Pool() noexcept = default;

    PoolRecordPointer<T> getPointer(const T &sample)
    {
        const auto key = sample.getString();
        const auto valueIter = records.find(key.toStdString());
        if(valueIter == records.end())
        {
            records.emplace(key.toStdString(), PoolRecord<T>(sample, this));
        }

        return PoolRecordPointer<T>(&records[key.toStdString()]);
    }

    PoolRecordPointer<T> getPointer(T &&sample)
    {
        const auto key = sample.getString();
        const auto valueIter = records.find(key.toStdString());
        if(valueIter == records.end())
        {
            records.emplace(key.toStdString(), PoolRecord<T>(std::move(sample), this));
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
    PoolRecord() = default;
    PoolRecord(const PoolRecord &other) = delete;
    PoolRecord(PoolRecord &&) noexcept = default;
    PoolRecord &operator =(const PoolRecord &) = delete;
    PoolRecord &operator =(PoolRecord &&) noexcept = delete;
    ~PoolRecord() noexcept = default;

    const T &getObject() const
    {
        return object;
    }

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

    bool operator!=(const PoolRecord &other)
    {
        return !(*this == other);
    }

private:
    PoolRecord(T object, Pool<T> *parent) :
        object(std::move(object)),
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
    Pool<T> *parent; //So Pool Record Pointer can access the pool to deallocate the record

    friend class Pool<T>;
    friend class PoolRecordPointer<T>;
};


template <class T>
class PoolRecordPointer
{
public:
    PoolRecordPointer() = default;

    PoolRecordPointer(PoolRecord<T> *ptr) :
        ptr(ptr)
    {
        ptr->incrementCounter();
    }

    PoolRecordPointer(const PoolRecordPointer &other) :
        ptr(other.ptr)
    {
        ptr->incrementCounter();
    }

    PoolRecordPointer &operator=(const PoolRecordPointer &other)
    {
        if(ptr != nullptr) //Needed due to default constructor
        {
            decrementAndRecountPointer();
        }

        ptr = other.ptr; //Change Record
        ptr->incrementCounter(); //Increment New Record Counter

        return *this;
    }

    PoolRecordPointer(PoolRecordPointer &&other) noexcept :
        ptr(other.ptr)
    {
        ptr->incrementCounter();
    }

    PoolRecordPointer &operator=(PoolRecordPointer &&other) noexcept
    {
        if(ptr != nullptr) //Needed due to default constructor
        {
            decrementAndRecountPointer();
        }

        ptr = other.ptr; //Change Record
        ptr->incrementCounter(); //Increment New Record Counter

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

    ~PoolRecordPointer() noexcept
    {
        decrementAndRecountPointer();
    }

private:
    void decrementAndRecountPointer()
    {
        ptr->decrementCounter();
        if(ptr->counterIsZero())
        {
            ptr->parent->records.erase(ptr->getObject().getString().toStdString());
        }
    }

    PoolRecord<T> *ptr = nullptr; //To test whether the pointer has been initialized given
                                  //it must be default constructible to support QVector
};

#endif // POOL_H
