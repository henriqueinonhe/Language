#ifndef FORMULA_H
#define FORMULA_H

#include "tokenstring.h"
#include "type.h"

class Parser;
class ProofLinks;

class Formula
{
public:
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

#endif // FORMULA_H

