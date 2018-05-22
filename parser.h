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
#include "parsingauxiliarytools.h"
#include "variabletoken.h"
#include "bindingtoken.h"
#include <iostream>

using namespace std;

class Parser
{
public:
    Parser(Signature *signature, const Type &wellFormedFormulaType);
    Formula parse(const QString &sentence);

private:
    struct ArgumentOffsets
    {
        ArgumentOffsets(const unsigned int beginOffset,
                        const unsigned int endOffset) :
            beginOffset(beginOffset),
            endOffset(endOffset)
        {
        }

        ArgumentOffsets(){}

        unsigned int beginOffset;
        unsigned int endOffset;
    };

    //Parsing Methods
    void buildParsingTree(const QString &sentence);

    void parseSentence(ParsingTreeIterator iter);
    void parseApplication(ParsingTreeIterator iter);
    void analyzeError(ParsingTreeIterator iter);

    bool isAtomic(const TokenString &tokenString) const;
    bool hasMolecularForm(const TokenString &tokenString) const;
    bool isDelimiter(const Token &token) const;
    bool outermostParenthesisMismatch(const TokenString &tokenString) const;

    QVector<ArgumentOffsets> separateArgumentOffsets(ParsingTreeIterator iter) const;
    void setArgumentsTypes(QVector<TypeTokenString> &argumentsTypes, ParsingTreeIterator &iter);

    //Type Checking Methods
    void performTypeChecking();
    void checkType(ParsingTreeIterator iter);

    //Variable Binding Checking Methods
    void performVariableBindingChecking();
    QVector<QVector<ParsingTreeNode *>> orderNodesByLevel() const;
    bool isVariableToken(const TokenString &tokenString) const;
    bool nodeHasBindingTokenAtChildren(const ParsingTreeNode *node) const;
    void performVariableBinding(ParsingTreeNode *parentNode);
    void propagateFreeAndBoundVariables(ParsingTreeNode *parentNode);

    const Type setMainOperatorType(ParsingTreeIterator iter);

    Lexer lexer;
    Type wellFormedFormulaType;
    unique_ptr<ParsingTree> parsingTree;
};

#endif // PARSER_H
