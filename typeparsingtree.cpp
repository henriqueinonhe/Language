#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"

TypeParsingTree::TypeParsingTree(const TypeTokenString &type) :
    root(this, nullptr, QVector<unsigned int>(), 0, type.size() - 1),
    height(0),
    typeString(type)
{
}

unsigned int TypeParsingTree::getHeight() const
{
    return height;
}

QString TypeParsingTree::print()
{
    TypeParsingTreeIterator iter(this);
    QVector<unsigned int> coord{};
    int currentHeight = 0;
    QString str;

    while(currentHeight < (int) this->getHeight()) //Each run prints all the nodes at a certain tree height
    {
        while(coord.size() < currentHeight - 1) //"Getting to" the right height
        {
            coord.push_back(0);
            label1:
            if(coord.back() < iter->getChildrenNumber())
            {
                iter.goToChild(coord.back());
            }
            else
            {
                if(!coord.isEmpty())
                {
                    coord.pop_back();
                    iter.goToParent();
                    coord.back()++;
                    goto label1;
                }
                else
                {
                    str += "\n\n";
                    currentHeight++;
                    break;
                }
            }
        }
        coord.push_back(0);
        while(coord.back() < iter->getChildrenNumber()) //Printing
        {
            iter.goToChild(coord.back());
            str += "(";
            if(iter->getCoordinates().size() >= 2)
            {
                str += QString::number(iter->getCoordinates()[iter->getCoordinates().size() - 2]);
            }
            str += ",";
            str += QString::number(iter->getCoordinates().back());
            str += "){";
            str += iter->getTypeString().toString();
            str += "} ";
            iter.goToParent();
            coord.back()++;
        }
        coord.pop_back();
        iter.goToParent();
        if(!coord.isEmpty())
        {
            coord.back()++;
        }
        else
        {
            str += "\n\n";
            currentHeight++;
        }
    }


    return str;
}
