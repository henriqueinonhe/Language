#include "formula.h"

Formula::Formula(QDataStream &stream, const Signature *signature) :
    tokenString(stream, signature)
{
}

bool Formula::operator==(const Formula &other) const
{
    return this->tokenString == other.tokenString;
}

bool Formula::operator!=(const Formula &other) const
{
    return !(*this == other);
}

QString Formula::toString() const
{
    return tokenString.toString();
}

QString Formula::formattedString() const
{
    return tokenString.formattedString();
}

Formula::Formula()
{

}

Formula::Formula(const TokenString &tokenString) :
    tokenString(tokenString)
{

}
