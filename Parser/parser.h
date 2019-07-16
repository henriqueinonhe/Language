#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include "lexer.h"
#include "type.h"

using namespace std;

class Signature;
class Formula;
class ParsingTree;
class ParsingTreeIterator;

class Parser
{
public:
    Parser() = delete;
    Parser(Signature * const signature, const Type &wellFormedFormulaType);
    Parser(const Parser &) = delete;

    Parser &operator =(const Parser &) = delete;

    Formula parse(const QString &sentence) const;
    Signature * getSignature() const;

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
    void buildParsingTree(const QString &sentence) const;

    void parseSentence(ParsingTreeIterator currentNodeIter) const;
    void parseApplication(ParsingTreeIterator currentNodeIter) const;
    [[noreturn]] void analyzeError(ParsingTreeIterator iter) const;

    bool isAtomic(const TokenString &tokenString) const;
    bool hasMolecularForm(const TokenString &tokenString) const;
    bool isDelimiter(const Token &token) const;
    bool outermostParenthesisMismatch(const TokenString &tokenString) const;
    void checkMinimumApplicationArgumentNumber(const QVector<ArgumentOffsets> &argumentsOffsets, ParsingTreeIterator currentNodeIter, const TokenString &tokenString) const;

    QVector<ArgumentOffsets> separateArgumentOffsets(ParsingTreeIterator currentNodeIter) const;
    void setArgumentsTypes(QVector<Type> &argumentsTypes, ParsingTreeIterator &currentNodeIter) const;
    void appendArgumentsNodes(const QVector<ArgumentOffsets> &argumentsOffsets, ParsingTreeIterator currentNodeIter) const;
    void parseArgumentsNodes(ParsingTreeIterator currentNodeIter) const;

    //Type Checking Methods
    void performTypeChecking() const;
    void checkType(ParsingTreeIterator iter) const;

    //Variable Binding Checking Methods
    void performVariableBindingChecking() const;
    QVector<QVector<ParsingTreeNode *>> orderNodesByLevel() const;
    bool isVariableToken(const TokenString &tokenString) const;
    bool nodeHasBindingTokenAtChildren(const ParsingTreeNode *node) const;
    void performVariableBinding(ParsingTreeNode *parentNode) const;
    void propagateFreeAndBoundVariables(ParsingTreeNode *parentNode) const;

    const Type setMainOperatorType(ParsingTreeIterator iter) const;
    void setVariablesNodes(QVector<QVector<ParsingTreeNode *> > &nodesMatrix) const;

    Lexer lexer;
    Type wellFormedFormulaType;
    mutable unique_ptr<ParsingTree> parsingTree;
};

#endif // PARSER_H
