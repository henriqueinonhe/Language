#ifndef TABLESIGNATURE_H
#define TABLESIGNATURE_H

#include "signature.h"
#include <memory>
#include <unordered_map>
#include <string>
#include "token.h"

using namespace std;

class TableSignature : public Signature
{
public:
    TableSignature();
    TableSignature(QDataStream &stream);

    virtual const Token *getTokenPointer(const QString &token);

    void addToken(const Token &token);

    bool operator==(const TableSignature &other) const;
    bool operator!=(const TableSignature &other) const;

    bool equalTokenTable(const TableSignature &other) const;

protected:
    virtual void serialize(QDataStream &stream) const;
    virtual void unserialize(QDataStream &stream);

private:
    bool tokenIsAlreadyPresentInSignature(const Token &token) const;
    void pushTokenPointerToTable(const Token &token);

    unordered_map<string, unique_ptr<Token>> tokenTable;

    friend QDataStream &operator <<(QDataStream &stream, const TableSignature &signature);
    friend QDataStream &operator >>(QDataStream &stream, TableSignature &signature);
};

QDataStream &operator <<(QDataStream &stream, const TableSignature &signature);
QDataStream &operator >>(QDataStream &stream, TableSignature &signature);
QDataStream &operator <<(QDataStream &stream, const unique_ptr<Token> &token);
QDataStream &operator >>(QDataStream &stream, unique_ptr<Token> &token);
QDataStream &operator <<(QDataStream &stream, const unordered_map<string, unique_ptr<Token>> &map);
QDataStream &operator >>(QDataStream &stream, unordered_map<string, unique_ptr<Token>> &map);

#endif // TABLESIGNATURE_H
