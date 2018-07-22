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

    bool operator==(const Formula &other) const;
    bool operator!=(const Formula &other) const;

    QString toString() const;
    QString formattedString() const;

private:
    Formula();
    Formula(const ParsingTree * const tree);

    unique_ptr<const ParsingTree> parsingTree;

    friend class Parser;
    friend class QVector<Formula>;
    friend class QVector<Formula>;
    friend QDataStream &operator <<(QDataStream &stream, const Formula &formula);
};

QDataStream &operator <<(QDataStream &stream, const Formula &formula);


#endif // FORMULA_H

