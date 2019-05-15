#ifndef FORMULA_H
#define FORMULA_H

#include <memory>

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

