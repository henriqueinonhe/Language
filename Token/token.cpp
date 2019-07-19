#include "token.h"

#include <QDataStream>
#include "punctuationtoken.h"
#include "variabletoken.h"
#include "coretoken.h"
#include "bindingtoken.h"
#include <typeinfo>
#include "qtclassesdeserialization.h"

Token::Token(const QString &string) :
    string(string)
{

}

bool Token::isEqual(const Token &other) const
{
    return this->string == other.string;
}

QDataStream &operator <<(QDataStream &stream, const Token &token)
{
    token.serialize(stream);

    return stream;
}

Token *Token::deserializePtr(QDataStream &stream)
{
    QString tokenType;
    stream >> tokenType;

    if(tokenType == "PunctuationToken")
    {
        return new PunctuationToken(stream);
    }
    else if(tokenType == "CoreToken")
    {
        return new CoreToken(stream);
    }
    else if(tokenType == "VariableToken")
    {
        return new VariableToken(stream);
    }
    else if(tokenType == "BindingToken")
    {
        return new BindingToken(stream);
    }
    else
    {
        throw std::invalid_argument("Cannot deserialize this token class!");
    }
}

QString Token::getString() const
{
    return string;
}

bool Token::operator==(const Token &other) const
{
    if(typeid(*this) != typeid(other))
    {
        return false;
    }
    else
    {
        return this->isEqual(other);
    }
}

bool Token::operator!=(const Token &other) const
{
    return !(other == *this);
}

unsigned int Token::getTokenCharacterSpan() const
{
    return static_cast<unsigned int>(string.size());
}

QString Token::tokenClass() const
{
    return "Token";
}

unique_ptr<Token> Token::getAllocatedClone() const
{
    //This is due to polymorphic Token creation
    return unique_ptr<Token>(new Token(*this));
}

void Token::serialize(QDataStream &stream) const
{
    stream << tokenClass() << string;
}

Token::Token(QDataStream &stream) :
    string(QtDeserialization::deserializeQString(stream))
{
}
