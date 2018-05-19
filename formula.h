#ifndef FORMULA_H
#define FORMULA_H

#include "tokenstring.h"
#include "type.h"

class Parser;

class Formula
{
public:
    bool operator==(const Formula &other) const;
    bool operator!=(const Formula &other) const;

private:
    Formula(const TokenString &tokenString);

    TokenString tokenString;

    friend class Parser;
};

#endif // FORMULA_H

