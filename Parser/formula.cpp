#include "formula.h"
#include "parsingtree.h"
#include "parsingtreeiterator.h"
#include "parser.h"
#include <QDataStream>

Formula::Formula(QDataStream &stream, const Signature *signature)
{
    Type wffType(stream);
    QString formattedString;
    stream >> formattedString;

    Parser parser(signature, wffType);
    Formula formula = parser.parse(formattedString);

    parsingTree.reset(formula.parsingTree.release());
}

QLinkedList<Formula> Formula::unserializeList(QDataStream &stream, const Signature *signature)
{
    int size;
    stream >> size;

    QLinkedList<Formula> list;
    for(int index = 0; index < size; index++)
    {
        list.push_back(Formula(stream, signature));
    }

    return list;
}

Formula::Formula(const Formula &other) :
    parsingTree(new ParsingTree(other.getParsingTree()))
{
}

bool Formula::operator==(const Formula &other) const
{
    return parsingTree->getTokenString() == other.parsingTree->getTokenString();
}

bool Formula::operator!=(const Formula &other) const
{
    return !(*this == other);
}

Type Formula::getWffType() const
{
    ParsingTreeIterator iter(const_cast<ParsingTree *>(parsingTree.get()));

    return iter->getType();
}

const ParsingTree &Formula::getParsingTree() const
{
    return *parsingTree;
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
    stream << formula.getWffType() << formula.formattedString();

    return stream;
}

QDataStream &operator <<(QDataStream &stream, const QLinkedList<Formula> &list)
{
    stream << list.size();
    std::for_each(list.begin(), list.end(), [&stream](const Formula &formula)
    {
        stream << formula;
    });

    return stream;
}
