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
    unsigned int currentHeight = 1;
    QString str;



    /* Desperate tryout */

    //
    str += "(){";
    str += iter->getTypeString().toString();
    str += "}\n\n";
    //
    if(!iter->isChildless())
    {
        for(unsigned int i = 0; i < iter->getChildrenNumber(); i++)
        {
            iter.goToChild(i);
            str += "(,";
            str += QString::number(iter->getOwnChildNumber());
            str += "){";
            str += iter->getTypeString().toString();
            str += "}";

            iter.goToParent();
        }

        str += "\n\n";
    }

    iter.goToRoot();

    while(currentHeight <= this->getHeight())
    {
        unsigned int heightCounter = 0;
        while(heightCounter < currentHeight)
        {
            if(!iter->isChildless())
            {
                iter.goToChild(0);
                coord.push_back(0);
                currentHeight++;
            }
            else
            {
                iter.goToParent();
                currentHeight--;
                //There is something here missing!
            }

            label1:
            iter->gatherChidrenStrings(str);
            iter.goToParent();
            coord.back()++;
            if(coord.back() < iter->getChildrenNumber())
            {
                iter.goToChild(coord.back());
                goto label1;
            }
            else
            {
                coord.pop_back();
                coord.back()++;
                //Something Missing Here
            }
        }

    }

    return str;
}
