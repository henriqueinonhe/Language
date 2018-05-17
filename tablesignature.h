#ifndef TABLESIGNATURE_H
#define TABLESIGNATURE_H

#include "signature.h"

class TableSignature : public Signature
{
public:

    TableSignature();
    Token *getTokenPointer(const QString &token);

    QVector<Token *> getTokenTable() const;
    void setTokenTable(const QVector<Token *> &value);

    void addToken(Token *token);

private:

    QVector<Token *> tokenTable; //NOTE maybe rework this!

};

#endif // TABLESIGNATURE_H
