#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <memory>

class Token
{
public:
    QString getString() const;

    virtual bool operator==(const Token &other) const;
    virtual bool operator!=(const Token &other) const;

    unsigned int getTokenCharSpan() const;

protected:
    Token(const QString &string);
    QString string;
};

#endif // TOKEN_H
