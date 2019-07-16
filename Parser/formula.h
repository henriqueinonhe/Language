#ifndef FORMULA_H
#define FORMULA_H

#include <memory>
#include "parsingtree.h"

template <class T> class QLinkedList;
template <class T> class QVector;
class Type;
class ParsingTree;
class QString;
class Signature;
class QDataStream;
class Parser;
class ProofLinks;

using namespace std;

class Formula
{
public:
    Formula(QDataStream &stream, Signature * const signature);
    Formula(const QString &formula, const Parser &parser);
    static QLinkedList<Formula> deserializeList(QDataStream &stream, Signature * const signature); //Use templates later
    static QVector<Formula> deserializeVector(QDataStream &stream, Signature * const signature);
    static Formula deserialize(QDataStream &stream, Signature * const signature);
    Formula(const Formula &other);

    Formula &operator=(const Formula &other);

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

    friend class Proof;
    friend class LineOfProof;
    friend class Parser;
    friend class QVector<Formula>;
    friend class QLinkedList<Formula>;
    friend QDataStream &operator <<(QDataStream &stream, const Formula &formula);
    friend QDataStream &operator <<(QDataStream &stream, const QLinkedList<Formula> &list);
    friend QDataStream &operator >>(QDataStream &stream, QLinkedList<Formula> &list);
    friend QDataStream &operator <<(QDataStream &stream, const QVector<Formula> &list);
    friend QDataStream &operator >>(QDataStream &stream, QVector<Formula> &list);
};

QDataStream &operator <<(QDataStream &stream, const Formula &formula);
QDataStream &operator <<(QDataStream &stream, const QLinkedList<Formula> &list);
QDataStream &operator <<(QDataStream &stream, const QVector<Formula> &vec);

#endif // FORMULA_H

