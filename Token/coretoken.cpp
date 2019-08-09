#include "coretoken.h"
#include <QRegularExpression>

CoreToken::CoreToken(QDataStream &stream) :
    Token(stream),
    type(stream)
{
}

CoreToken::CoreToken(const QString &string, const Type &type) :
    Token(validateCoreTokenString(string)),
    type(type)
{

}

Type CoreToken::getType() const
{
    return type;
}

QString CoreToken::tokenClass() const
{
    return "CoreToken";
}

unique_ptr<Token> CoreToken::getAllocatedClone() const
{
    return unique_ptr<Token>(new CoreToken(*this));
}

void CoreToken::serialize(QDataStream &stream) const
{
    Token::serialize(stream);
    stream << type;
}

bool CoreToken::isEqual(const Token &other) const
{
    return Token::isEqual(other) &&
            this->type == static_cast<const CoreToken &>(other).type;
}

QString CoreToken::validateCoreTokenString(const QString &tokenString) const
{
    if(!QRegularExpression("^([a-z]|[A-Z]|[0-9]|\\+|\\-|\\\\|\\*|&|-|>|<|\\||=|\\~|\\^)+$").match(tokenString).hasMatch())
    {
        QString errorMsg;
        errorMsg += "This (\"";
        errorMsg += tokenString;
        errorMsg += "\") is not a suitable Core Token!";
        throw std::invalid_argument(errorMsg.toStdString());
    }
    return tokenString;
}

QDataStream &operator <<(QDataStream &stream, const CoreToken &token)
{
    token.serialize(stream);
    return stream;
}
