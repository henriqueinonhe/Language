#include "tree.h"

Tree::Tree() :
    root(this, nullptr, QVector<unsigned int>()),
    height(0)
{

}

unsigned int Tree::getHeight() const
{
    return height;
}
