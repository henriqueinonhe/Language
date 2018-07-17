#ifndef TABLESIGNATURE_H
#define TABLESIGNATURE_H

#include "signature.h"
#include "punctuationtoken.h"
#include <memory>

using namespace std;

class TableSignature : virtual public Signature
{
public:
    TableSignature();
    const Token *getTokenPointer(const QString &token) const;

    void addToken(const Token &token);

private:
    bool tokenIsAlreadyPresentInSignature(const Token &token) const;

    QVector<shared_ptr<Token>> tokenTable;
    void pushTokenPointerToTable(const Token &token);
};

#endif // TABLESIGNATURE_H
