#include "token.h"

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

QDataStream &operator >>(QDataStream &stream, Token &token)
{
    token.unserialize(stream);

    return stream;
}

shared_ptr<Token> Token::unserializePtr(QDataStream &stream)
{
    QString tokenType;
    stream >> tokenType;

    if(tokenType == "PunctuationToken")
    {
        PunctuationToken token;
        stream >> token;

        return make_shared<PunctuationToken>(token);
    }
    else if(tokenType == "CoreToken")
    {
        CoreToken token;
        stream >> token;

        return make_shared<CoreToken>(token);
    }
    else if(tokenType == "VariableToken")
    {
        VariableToken token;
        stream >> token;

        return make_shared<VariableToken>(token);
    }
    else if(tokenType == "BindingToken")
    {
        BindingToken token;
        stream >> token;

        return make_shared<BindingToken>(token);
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

unsigned int Token::getTokenCharSpan() const
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

void Token::unserialize(QDataStream &stream)
{
    stream >> string;
}

Token::Token()
{

}
