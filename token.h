#ifndef TOKEN_H
#define TOKEN_H

#include <QString>

class Token
{
public:
    Token();
    Token(QString string);
    QString getString() const;

    virtual bool operator==(const Token &other) const;

private:
    QString string;

};

#endif // TOKEN_H
