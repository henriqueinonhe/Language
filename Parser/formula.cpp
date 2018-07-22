#include "formula.h"

#include "parser.h"

Formula::Formula(QDataStream &stream, const Signature *signature)
{
    Type wffType(stream);
    QString formattedString;
    stream >> formattedString;

    Parser parser(signature, wffType);
    Formula formula = parser.parse(formattedString);

    parsingTree.reset(formula.parsingTree.release());
}

bool Formula::operator==(const Formula &other) const
{
    return parsingTree->getTokenString() == other.parsingTree->getTokenString();
}

bool Formula::operator!=(const Formula &other) const
{
    return !(*this == other);
}

QString Formula::toString() const
{
    return parsingTree->getTokenString().toString();
}

QString Formula::formattedString() const
{
    return parsingTree->getTokenString().formattedString();
}

Formula::Formula()
{

}

Formula::Formula(const ParsingTree * const tree) :
    parsingTree(tree)
{
}

QDataStream &operator <<(QDataStream &stream, const Formula &formula)
{
    ParsingTreeIterator iter(const_cast<ParsingTree *>(formula.parsingTree.get()));

    stream << iter->getType() << formula.formattedString();

    return stream;
}
