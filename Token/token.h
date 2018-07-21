#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <memory>
#include <typeinfo>
#include <QDataStream>

using namespace std;

class PunctuationToken;
class CoreToken;
class BindingToken;
class VariableToken;

class Token
{
public:
    static shared_ptr<Token> unserializePtr(QDataStream &stream);

    QString getString() const;

    bool operator==(const Token &other) const;
    bool operator!=(const Token &other) const;

    unsigned int getTokenCharSpan() const;

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;

    virtual void serialize(QDataStream &stream) const;
    virtual void unserialize(QDataStream &stream);

protected:
    Token();
    Token(const QString &string);

    virtual bool isEqual(const Token &other) const;

    QString string;

    friend QDataStream &operator <<(QDataStream &stream, const Token &token);
    friend QDataStream &operator >>(QDataStream &stream, Token &token);
};

QDataStream &operator <<(QDataStream &stream, const Token &token);
QDataStream &operator >>(QDataStream &stream, Token &token);

#endif // TOKEN_H
