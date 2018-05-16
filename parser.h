#ifndef PARSER_H
#define PARSER_H

#include "signature.h"
#include "lexer.h"
#include <memory>
#include "formula.h"
#include "parsingtree.h"
#include "parsingerrorexception.hpp"
#include "parsingtreeiterator.h"
#include "parsingtreenode.h"

using namespace std;

class Parser
{
public:
    Parser(Signature *signature, const Type &wellFormedFormulaType);
    Formula parse(const QString &sentence);

private:
    void buildParsingTree(const QString &sentence);

    Lexer lexer;
    Type wellFormedFormulaType;
    unique_ptr<ParsingTree> parsingTree;
};

#endif // PARSER_H
