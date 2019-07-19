#include "typetoken.h"
#include <QRegularExpression>
#include "qtclassesdeserialization.h"

TypeToken::TypeToken(QDataStream &stream) :
    string(QtDeserialization::deserializeQString(stream))
{
}

TypeToken::TypeToken(const QString &string) :
    string(string)
{
    //Validating input
    sort(string);
}

TypeToken::Sort TypeToken::sort() const
{
    return sort(string);
}

TypeToken::Sort TypeToken::sort(const QString &string)
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
    return static_cast<unsigned int>(string.size());
}


QDataStream &operator <<(QDataStream &stream, const TypeToken &token)
{
    stream << token.string;

    return stream;
}
