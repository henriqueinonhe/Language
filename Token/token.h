#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <memory>
#include <typeinfo>

using namespace std;

class PunctuationToken;
class CoreToken;
class BindingToken;
class VariableToken;

class Token
{
public:
    static Token *deserializePtr(QDataStream &stream);

    QString getString() const;

    Token &operator =(const Token &other) = delete;

    bool operator==(const Token &other) const;
    bool operator!=(const Token &other) const;

    unsigned int getTokenCharacterSpan() const;

    virtual QString tokenClass() const;

    virtual Token *getAllocatedClone() const;

    virtual ~Token();


protected:
    Token(QDataStream &stream);
    Token(const QString &string);

    virtual bool isEqual(const Token &other) const;

    virtual void serialize(QDataStream &stream) const;

    QString string;

    friend QDataStream &operator <<(QDataStream &stream, const Token &token);
};

QDataStream &operator <<(QDataStream &stream, const Token &token);

#endif // TOKEN_H
