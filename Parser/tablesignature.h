#ifndef TABLESIGNATURE_H
#define TABLESIGNATURE_H

#include "signature.h"
#include <memory>
#include <QVector>

using namespace std;

class TableSignature : public Signature
{
public:
    TableSignature(QDataStream &stream);
    TableSignature();

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

    QVector<shared_ptr<Token>> tokenTable;

    friend QDataStream &operator <<(QDataStream &stream, const TableSignature &signature);
    friend QDataStream &operator >>(QDataStream &stream, TableSignature &signature);
};

QDataStream &operator <<(QDataStream &stream, const TableSignature &signature);
QDataStream &operator >>(QDataStream &stream, TableSignature &signature);
QDataStream &operator <<(QDataStream &stream, const shared_ptr<Token> &token);
QDataStream &operator >>(QDataStream &stream, shared_ptr<Token> &token);

#endif // TABLESIGNATURE_H
