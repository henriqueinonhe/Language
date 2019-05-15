#include "token.h"

#include <QDataStream>
#include "punctuationtoken.h"
#include "variabletoken.h"
#include "coretoken.h"
#include "bindingtoken.h"

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

shared_ptr<Token> Token::unserializePtr(QDataStream &stream)
{
    QString tokenType;
    stream >> tokenType;

    if(tokenType == "PunctuationToken")
    {
        return make_shared<PunctuationToken>(PunctuationToken(stream));
    }
    else if(tokenType == "CoreToken")
    {
        return make_shared<CoreToken>(CoreToken(stream));
    }
    else if(tokenType == "VariableToken")
    {
        return make_shared<VariableToken>(VariableToken(stream));
    }
    else if(tokenType == "BindingToken")
    {
        return make_shared<BindingToken>(BindingToken(stream));
    }
    else
    {
        throw std::invalid_argument("Cannot unserialize this token class!");
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
    return string.size();
}

QString Token::tokenClass() const
{
    return "Token";
}

Token *Token::getAllocatedClone() const
{
    return new Token(*this);
}

void Token::serialize(QDataStream &stream) const
{
    stream << tokenClass() << string;
}

Token::Token(QDataStream &stream)
{
    stream >> string;
}
