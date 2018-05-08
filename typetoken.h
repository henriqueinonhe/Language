#ifndef TYPETOKEN_H
#define TYPETOKEN_H

#include <QString>
#include <QRegularExpression>

class TypeToken
{
public:
    enum class Sort
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


private:
    void assignSort(const QString &string);

    Sort sort;
    QString string;

};

#endif // TYPETOKEN_H
