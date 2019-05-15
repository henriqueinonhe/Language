#include "typetoken.h"
#include <QRegularExpression>

TypeToken::TypeToken()
{

}

TypeToken::TypeToken(QDataStream &stream)
{
    stream >> string;
}

TypeToken::TypeToken(const QString &string) :
    string(string)
{
    //Validating input
    getSort();
}

TypeToken::Sort TypeToken::getSort() const
{
    if(string == "(")
    {
        return Sort::LeftParenthesis;
    }
    else if(string == ")")
    {
        return Sort::RightParenthesis;
    }
    else if(string == "[")
    {
        return Sort::LeftSquareBracket;
    }
    else if(string == "]")
    {
        return Sort::RightSquareBracket;
    }
    else if(string == ",")
    {
        return Sort::Comma;
    }
    else if(string == "->")
    {
        return Sort::CompositionOperator;
    }
    else if(QRegularExpression("^([a-z]|[A-Z]|[0-9])+$").match(string).hasMatch())
    {
        return Sort::PrimitiveType;
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

bool TypeToken::operator ==(const TypeToken &other) const
{
    return this->getString() == other.getString();
}

bool TypeToken::operator !=(const TypeToken &other) const
{
    return !(*this == other);
}

QString TypeToken::getString() const
{
    return string;
}

unsigned int TypeToken::getTokenCharacterSpan() const
{
    return string.size();
}


QDataStream &operator <<(QDataStream &stream, const TypeToken &token)
{
    stream << token.string;

    return stream;
}
