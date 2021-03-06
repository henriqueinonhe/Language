#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <memory>

using namespace std;

class PunctuationToken;
class CoreToken;
class BindingToken;
class VariableToken;

class Token
{
public:
    Token() = delete;
    Token &operator =(const Token &other) = delete;
    Token &operator =(Token &&) = delete;
    virtual ~Token() noexcept = default;

    Token(QDataStream &stream);
    Token(const QString &string);
    Token(QString &&string);

    static Token *deserializePtr(QDataStream &stream);

    QString getString() const;

    bool operator==(const Token &other) const;
    bool operator!=(const Token &other) const;

    unsigned int getTokenCharacterSpan() const;

    virtual QString tokenClass() const;

    virtual unique_ptr<Token> getAllocatedClone() const;

protected:
    Token(const Token &other) = default;
    Token(Token &&) noexcept = default;

    virtual bool isEqual(const Token &other) const;
    virtual void serialize(QDataStream &stream) const;

private:
    QString string;

    friend QDataStream &operator <<(QDataStream &stream, const Token &token);
};

QDataStream &operator <<(QDataStream &stream, const Token &token);

#endif // TOKEN_H
