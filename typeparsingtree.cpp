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
    QVector<unsigned int> coord{0};
    unsigned int currentHeight = 1;
    QString str;

    //This is the ad-hoc part of the code.
    str += "(){";
    str += iter->getTypeString().toString();
    str += "}\n\n";

    //rewrite this
    if(!iter->isChildless())
    {
        label1:
        iter.goToChild(coord.back());
        str += "(,";
        str += QString::number(iter->getCoordinates().back());
        str += "){\"";
        str += iter->getTypeString().toString();
        str += "\"} ";

        iter.goToParent();
        coord.back()++;

        if(coord.back() < iter->getChildrenNumber())
        {
            goto label1;
        }
        else
        {
            currentHeight++;
        }
    }

    //temporary fix
    coord.back() = 0;

    //real function
    unsigned int heightCounter = 0;
    while(heightCounter < currentHeight)
    {
        label2:
        iter.goToChild(coord.back());
        iter->gatherChidrenStrings(str);

        iter.goToParent();
        coord.back()++;

        if(coord.back() < iter->getChildrenNumber())
        {
            goto label2;
        }
        else
        {
            coord.pop_back();
            coord.back()++;
        }
    }



    /* Desperate tryout */




    return str;
}
