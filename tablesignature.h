#ifndef TABLESIGNATURE_H
#define TABLESIGNATURE_H

#include "signature.h"

class TableSignature : public Signature
{
public:

    TableSignature();
    Token *getTokenPointer(const QString &token);

    void addToken(Token *token);

private:

    QVector<Token *> tokenTable; //FIXME Fix this URGENTLY!

};

#endif // TABLESIGNATURE_H
