#ifndef POOL_H
#define POOL_H

#include <QVector>
#include <QLinkedList>
#include <unordered_map>
#include <string>
#include <memory>

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
            records.emplace(key.toStdString(), make_shared<PoolRecord<T>>(sample));
        }

        return PoolRecordPointer<T>(records[key.toStdString()]);
    }

    PoolRecordPointer<T> getPointer(T &&sample)
    {
        const auto key = sample.getString();
        const auto valueIter = records.find(key.toStdString());
        if(valueIter == records.end())
        {
            records.emplace(key.toStdString(), make_shared<PoolRecord<T>>(std::move(sample)));
        }

        return PoolRecordPointer<T>(records[key.toStdString()]);
    }

    unordered_map<string, shared_ptr<PoolRecord<T>>> &getRecords()
    {
        return records;
    }

private:
    unordered_map<string, shared_ptr<PoolRecord<T>>> records;

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

    bool operator==(const PoolRecord &other) const
    {
        return this->object == other.object &&
               this->parent == other.parent;
    }

    bool operator!=(const PoolRecord &other)
    {
        return !(*this == other);
    }

    PoolRecord(const T &object) :
        object(object)
    {
    }

    PoolRecord(T &&object) :
        object(std::move(object))
    {
    }

private:

    T object;

    friend class Pool<T>;
    friend class PoolRecordPointer<T>;
};


template <class T>
class PoolRecordPointer
{
public:
    PoolRecordPointer(const shared_ptr<PoolRecord<T>> &ptr) :
        ptr(ptr)
    {
    }

    PoolRecordPointer(shared_ptr<PoolRecord<T>> &&ptr) :
        ptr(std::move(ptr))
    {

    }

    PoolRecordPointer(const PoolRecordPointer &other) = default;

    PoolRecordPointer &operator=(const PoolRecordPointer &other)
    {
        ptr = other.ptr;
        return *this;
    }

    PoolRecordPointer(PoolRecordPointer &&other) noexcept = default;

    PoolRecordPointer &operator=(PoolRecordPointer &&other) noexcept = default;

    T operator*() const
    {
        return ptr->getObject();
    }

    T *operator->() const
    {
        return &ptr->getObject();
    }

private:
    shared_ptr<PoolRecord<T>> ptr;
};

#endif // POOL_H
