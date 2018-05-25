#ifndef TABLESIGNATURE_H
#define TABLESIGNATURE_H

#include "signature.h"
#include "punctuationtoken.h"
#include <memory>

using namespace std;

class TableSignature : public Signature
{
public:

    TableSignature();
    Token *getTokenPointer(const QString &token);

    void addToken(const Token &token);

private:

    QVector<shared_ptr<Token>> tokenTable;

};

#endif // TABLESIGNATURE_H
