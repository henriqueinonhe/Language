#ifndef SIGNATURE_H
#define SIGNATURE_H

class Token;
class QDataStream;
class QString;

class Signature
{
public:
    virtual const Token *getTokenPointer(const QString &token) = 0;
    virtual void addToken(const Token &token) = 0;

    virtual ~Signature() = 0;

protected:
    virtual void serialize(QDataStream &stream) const = 0;
    virtual void unserialize(QDataStream &stream) = 0;

    friend QDataStream &operator <<(QDataStream &stream, const Signature &signature);
    friend QDataStream &operator >>(QDataStream &stream, Signature &signature);
};

QDataStream &operator <<(QDataStream &stream, const Signature &signature);
QDataStream &operator >>(QDataStream &stream, Signature &signature);

#endif // SIGNATURE_H
