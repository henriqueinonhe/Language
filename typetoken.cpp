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
    else if(string == "{")
    {
        sort = Sort::LeftCurlyBracket;
    }
    else if(string == "}")
    {
        sort = Sort::RightCurlyBracket;
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
        message += "' -> ";
        message += "This is not a suitable expression for a type.";

        throw std::invalid_argument(message.toStdString());
    }
}

