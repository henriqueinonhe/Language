#include "dirtyfix.h"

#include "typeparser.h"

void DirtyFix::fix()
{
    TypeParser::parsingTree.reset();
}
