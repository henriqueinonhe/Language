#ifndef FORMULA_H
#define FORMULA_H

#include "tokenstring.h"
#include "type.h"
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
    Formula(const TokenString &tokenString);

    TokenString tokenString;

    friend class Parser;
    friend class QVector<Formula>;
};

QDataStream &operator <<(QDataStream &stream, const Formula &formula);


#endif // FORMULA_H

