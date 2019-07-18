#include "coretoken.h"
#include <QRegularExpression>

CoreToken::CoreToken(QDataStream &stream) :
    Token(stream),
    type(stream)
{
}

CoreToken::CoreToken(const QString &string, const Type &type) :
    Token(string),
    type(type)
{
    if(!QRegularExpression("^([a-z]|[A-Z]|[0-9]|\\+|\\-|\\\\|\\*|&|-|>|<|\\||=|\\~|\\^)+$").match(string).hasMatch())
    {
        QString errorMsg;
        errorMsg += "This (\"";
        errorMsg += string;
        errorMsg += "\") is not a suitable Core Token!";
        throw std::invalid_argument(errorMsg.toStdString());
    }
}

Type CoreToken::getType() const
{
    return type;
}

QString CoreToken::tokenClass() const
{
    return "CoreToken";
}

Token *CoreToken::getAllocatedClone() const
{
    return new CoreToken(*this);
}

CoreToken::~CoreToken()
{

}

void CoreToken::serialize(QDataStream &stream) const
{
    Token::serialize(stream);
    stream << type;
}

bool CoreToken::isEqual(const Token &other) const
{
    return this->type == static_cast<const CoreToken &>(other).type &&
           isEqual(static_cast<const CoreToken &>(other));
}

bool CoreToken::isEqual(const CoreToken &other) const
{
    return true;
}

QDataStream &operator <<(QDataStream &stream, const CoreToken &token)
{
    token.serialize(stream);

    return stream;
}
