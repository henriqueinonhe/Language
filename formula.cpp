#include "formula.h"

bool Formula::operator==(const Formula &other) const
{
    return this->tokenString == other.tokenString;
}

bool Formula::operator!=(const Formula &other) const
{
    return !(*this == other);
}

Formula::Formula(const TokenString &tokenString) :
    tokenString(tokenString)
{

}
