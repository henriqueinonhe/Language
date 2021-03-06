#include "formula.h"
#include "parsingtree.h"
#include "parsingtreeiterator.h"
#include "parser.h"
#include <QDataStream>

Formula::Formula(QDataStream &stream, const Signature * const signature)
{
    Type wffType(stream);
    QString formattedString;
    stream >> formattedString;

    Parser parser(signature, wffType);
    auto formula = parser.parse(formattedString);

    parsingTree.reset(formula.parsingTree.release());
}

Formula::Formula(const QString &formula, const Parser &parser) :
    parsingTree(new ParsingTree(parser.parse(formula).getParsingTree()))
{

}

QLinkedList<Formula> Formula::deserializeList(QDataStream &stream, Signature * const signature)
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

QVector<Formula> Formula::deserializeVector(QDataStream &stream, const Signature * const signature)
{
    int size;
    stream >> size;

    QVector<Formula> vec;
    for(int index = 0; index < size; index++)
    {
        vec.push_back(Formula(stream, signature));
    }

    return vec;
}

Formula Formula::deserialize(QDataStream &stream, Signature * const signature)
{
    Type wffType(stream);
    QString formattedString;
    stream >> formattedString;

    Parser parser(signature, wffType);
    return parser.parse(formattedString);
}

Formula::Formula(const Formula &other) :
    parsingTree(new ParsingTree(other.getParsingTree()))
{

}

Formula::Formula(Formula &&other) noexcept :
    parsingTree(other.parsingTree.release())
{

}

Formula &Formula::operator=(const Formula &other)
{
    parsingTree.reset(new ParsingTree(other.getParsingTree()));
    return *this;
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
    for(const auto &formula : list)
    {
        stream << formula;
    }

    return stream;
}


QDataStream &operator <<(QDataStream &stream, const QVector<Formula> &vec)
{
    stream << vec.size();
    for(const Formula &formula : vec)
    {
        stream << formula;
    }
    return stream;
}
