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
    TableSignature(const TableSignature &) = default; //Maybe this is problematic due to unique_ptr<Token>, test!
    TableSignature(TableSignature &&) noexcept = default;
    TableSignature &operator =(const TableSignature &) = default; //Idem
    TableSignature &operator =(TableSignature &&) noexcept = default;
    ~TableSignature() = default;

    TableSignature(QDataStream &stream);

    virtual const Token *getTokenPointer(const QString &token) override;

    void addToken(const Token &token);

    bool operator==(const TableSignature &other) const;
    bool operator!=(const TableSignature &other) const;

    bool equalTokenTable(const TableSignature &other) const;

protected:
    virtual void serialize(QDataStream &stream) const override;
    virtual void deserialize(QDataStream &stream) override;

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
