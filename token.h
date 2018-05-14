#ifndef TOKEN_H
#define TOKEN_H

#include <QString>
#include "type.h"
#include <memory>

class Token
{
public:
    Token();
    Token(QString string); //This should be reviwed!
    QString getString() const;

    virtual bool operator==(const Token &other) const;
    virtual bool operator!=(const Token &other) const;

private:
    QString string;
    std::shared_ptr<Type> type;

};

#endif // TOKEN_H
