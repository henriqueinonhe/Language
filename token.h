#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include <memory>
#include <typeinfo>

class Token
{
public:
    QString getString() const;

    bool operator==(const Token &other) const;
    bool operator!=(const Token &other) const;

    unsigned int getTokenCharSpan() const;

    virtual QString tokenClass() const;

    virtual Token *allocatedClone() const;

protected:
    Token(const QString &string);

    virtual bool isEqual(const Token &other) const;

    QString string;
};

#endif // TOKEN_H
