#ifndef TYPETOKEN_H
#define TYPETOKEN_H

#include <QString>

template <class T> class PoolRecord;

/* Class Invariant:
 * A TypeToken ALWAYS represent a valid TypeToken
 * and it holds that token until the end of its life,
 * that is, it is not mutable and therefore not copyable. */

class TypeToken final
{
public:
    enum class Sort : unsigned char
    {
        LeftParenthesis,
        RightParenthesis,
        LeftSquareBracket,
        RightSquareBracket,
        Comma,
        CompositionOperator,
        PrimitiveType
    };

    TypeToken(const TypeToken &other) = default;
    TypeToken(TypeToken &&) = default;
    TypeToken &operator =(const TypeToken &) = delete;
    TypeToken &operator =(TypeToken &&) = default;
    ~TypeToken() = default;

    TypeToken(QDataStream &stream);
    TypeToken(const QString &string);

    Sort sort() const;
    static Sort sort(const QString &string);

    bool operator ==(const TypeToken &other) const;
    bool operator !=(const TypeToken &other) const;

    QString getString() const;

    unsigned int getTokenCharacterSpan() const;

private:
    TypeToken() = default;
    const QString string;

    friend QDataStream &operator <<(QDataStream &stream, const TypeToken &token);
    friend class PoolRecord<TypeToken>;
};

QDataStream &operator <<(QDataStream &stream, const TypeToken &token);

#endif // TYPETOKEN_H
