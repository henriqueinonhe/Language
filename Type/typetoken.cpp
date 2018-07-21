#include "typetoken.h"

TypeToken::TypeToken()
{

}

TypeToken::TypeToken(const QString &string)
{
    assignSort(string);
    this->string = string;
}

TypeToken::Sort TypeToken::getSort() const
{
    return sort;
}

bool TypeToken::operator ==(const TypeToken &other) const
{
    return this->getString() == other.getString() &&
            this->sort == other.sort;
}

bool TypeToken::operator !=(const TypeToken &other) const
{
    return !(*this == other);
}

QString TypeToken::getString() const
{
    return string;
}

TypeToken TypeToken::setString(const QString &value)
{
    assignSort(value);
    string = value;

    return *this;
}

unsigned int TypeToken::getTokenCharSpan() const
{
    return string.size();
}

void TypeToken::assignSort(const QString &string)
{
    if(string == "(")
    {
        sort = Sort::LeftParenthesis;
    }
    else if(string == ")")
    {
        sort = Sort::RightParenthesis;
    }
    else if(string == "[")
    {
        sort = Sort::LeftSquareBracket;
    }
    else if(string == "]")
    {
        sort = Sort::RightSquareBracket;
    }
    else if(string == ",")
    {
        sort = Sort::Comma;
    }
    else if(string == "->")
    {
        sort = Sort::CompositionOperator;
    }
    else if(QRegularExpression("^([a-z]|[A-Z]|[0-9])+$").match(string).hasMatch())
    {
        sort = Sort::PrimitiveType;
    }
    else
    {
        QString message;
        message += "'";
        message += string;
        message += "' ";
        message += "is not a suitable expression for a primitive type.";

        throw std::invalid_argument(message.toStdString());
    }
}


QDataStream &operator <<(QDataStream &stream, const TypeToken &token)
{
    stream << (qint8 &) token.sort << token.string;

    return stream;
}

QDataStream &operator >>(QDataStream &stream, TypeToken &token)
{
    stream >> (qint8 &) token.sort >> token.string;

    return stream;
}
