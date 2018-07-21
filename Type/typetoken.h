#ifndef TYPETOKEN_H
#define TYPETOKEN_H

#include <QString>
#include <QRegularExpression>
#include <QDataStream>

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

    TypeToken();
    TypeToken(const QString &string);

    Sort getSort() const;

    bool operator ==(const TypeToken &other) const;
    bool operator !=(const TypeToken &other) const;

    QString getString() const;
    TypeToken setString(const QString &value);

    unsigned int getTokenCharSpan() const;

private:
    void assignSort(const QString &string);

    Sort sort;
    QString string;

    friend QDataStream &operator <<(QDataStream &stream, const TypeToken &token);
    friend QDataStream &operator >>(QDataStream &stream, TypeToken &token);

};

QDataStream &operator <<(QDataStream &stream, const TypeToken &token);
QDataStream &operator >>(QDataStream &stream, TypeToken &token);

#endif // TYPETOKEN_H
