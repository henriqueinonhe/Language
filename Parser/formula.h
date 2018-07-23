#ifndef FORMULA_H
#define FORMULA_H

#include "tokenstring.h"
#include "type.h"
#include "parsingtree.h"
#include <QDataStream>

class Parser;
class ProofLinks;

class Formula
{
public:
    Formula(QDataStream &stream, const Signature * signature);
    static QLinkedList<Formula> unserializeList(QDataStream &stream, const Signature * signature);
    Formula(const Formula &other);

    bool operator==(const Formula &other) const;
    bool operator!=(const Formula &other) const;


    Type getWffType() const;
    const ParsingTree &getParsingTree() const;

    QString toString() const;
    QString formattedString() const;

private:
    Formula();
    Formula(const ParsingTree * const tree);

    unique_ptr<const ParsingTree> parsingTree;

    friend class Parser;
    friend class QVector<Formula>;
    friend class QLinkedList<Formula>;
    friend QDataStream &operator <<(QDataStream &stream, const Formula &formula);
    friend QDataStream &operator <<(QDataStream &stream, const QLinkedList<Formula> &list);
};

QDataStream &operator <<(QDataStream &stream, const Formula &formula);
QDataStream &operator <<(QDataStream &stream, const QLinkedList<Formula> &list);

#endif // FORMULA_H

