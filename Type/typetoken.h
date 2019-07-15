#ifndef TYPETOKEN_H
#define TYPETOKEN_H

#include <QString>

template <class T> class PoolRecord;

class TypeToken
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

    TypeToken(QDataStream &stream);
    TypeToken(const QString &string);

    TypeToken(const TypeToken &other) = default;
    TypeToken &operator =(const TypeToken &) = delete;

    Sort sort() const;
    static Sort sort(const QString &string);

    bool operator ==(const TypeToken &other) const;
    bool operator !=(const TypeToken &other) const;

    QString getString() const;

    unsigned int getTokenCharacterSpan() const;

private:
    TypeToken() = default;
    QString string;

    friend QDataStream &operator <<(QDataStream &stream, const TypeToken &token);
    friend class PoolRecord<TypeToken>;
};

QDataStream &operator <<(QDataStream &stream, const TypeToken &token);

#endif // TYPETOKEN_H
