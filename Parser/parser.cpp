#include "parser.h"
#include "formula.h"
#include "parsingerrorexception.hpp"
#include "parsingtreeiterator.h"
#include "parsingauxiliarytools.h"
#include "variabletoken.h"
#include "bindingtoken.h"
#include "punctuationtoken.h"
#include "tokenstring.h"
#include "parsingtree.h"
#include "parsingtreenode.h"
#include <iostream>

Parser::Parser(Signature * const signature, const Type &wellFormedFormulaType) :
    lexer(signature),
    wellFormedFormulaType(wellFormedFormulaType)
{
}

Formula Parser::parse(const QString &sentence) const
{
    buildParsingTree(sentence);
    performTypeChecking();
    performVariableBindingChecking();

    //std::cout << parsingTree->print().toStdString();

    return Formula(parsingTree.release());
}

Signature *Parser::getSignature() const
{
    return lexer.getSignature();
}

void Parser::buildParsingTree(const QString &sentence) const
{
    const auto tokenString(lexer.lex(sentence));

    parsingTree.reset(new ParsingTree(tokenString));

    ParsingTreeIterator iter(parsingTree.get());

    parseSentence(iter);
}

[[noreturn]] void Parser::analyzeError(ParsingTreeIterator currentNodeIter) const
{
    const auto tokenString = currentNodeIter->getTokenString();
    const auto beginIndex = currentNodeIter->getBeginIndex();

    if(tokenString.first().getString() == ")")
    {
        throw ParsingErrorException<TokenString>("The first character cannot be a right parenthesis.",
                                                 beginIndex,
                                                 beginIndex,
                                                 currentNodeIter.goToRoot()->getTokenString());
    }
    else if(outermostParenthesisMismatch(tokenString))
    {
        const auto zeroIndexCompensation = 1;
        const auto endIndex = beginIndex + tokenString.size() - zeroIndexCompensation;
        throw ParsingErrorException<TokenString>("The outermost parenthesis doesn't match!",
                                                 beginIndex,
                                                 endIndex,
                                                 currentNodeIter.goToRoot()->getTokenString());
    }
    else if(!isDelimiter(tokenString.first()) && tokenString.size() != 1)
    {
        const auto zeroIndexCompensation = 1;
        const auto endIndex = beginIndex + tokenString.size() - zeroIndexCompensation;
        throw ParsingErrorException<TokenString>("A string with more than one token should be an application, which uses parenthesis and none was found!",
                                                 beginIndex,
                                                 endIndex,
                                                 currentNodeIter.goToRoot()->getTokenString());
    }
    else
    {
        throw std::logic_error("This case shouldn't happen in parsing!");
    }
}

void Parser::checkMinimumApplicationArgumentNumber(const QVector<ArgumentOffsets> &argumentsOffsets, ParsingTreeIterator currentNodeIter, const TokenString &tokenString) const
{
    const auto minimumApplicatonArgumentNumber = 2;
    if(argumentsOffsets.size() < static_cast<int>(minimumApplicatonArgumentNumber))
    {
        const auto beginIndex = currentNodeIter->getBeginIndex();
        const auto endIndex = currentNodeIter->getBeginIndex() + tokenString.size();

        throw ParsingErrorException<TokenString>("An application should have at least two arguments!",
                                                 beginIndex,
                                                 endIndex,
                                                 tokenString);
    }
}

void Parser::appendArgumentsNodes(const QVector<ArgumentOffsets> &argumentsOffsets, ParsingTreeIterator currentNodeIter) const
{
    for(const auto &offsets : argumentsOffsets)
    {
        const auto argumentBeginIndex = currentNodeIter->getBeginIndex() + offsets.beginOffset;
        const auto argumentEndIndex = currentNodeIter->getBeginIndex() + offsets.endOffset;
        currentNodeIter->appendChild(argumentBeginIndex, argumentEndIndex);
    }
}

void Parser::parseArgumentsNodes(ParsingTreeIterator currentNodeIter) const
{
    for(unsigned int childIndex = 0; childIndex < currentNodeIter->getChildrenNumber(); childIndex++)
    {
        currentNodeIter.goToChild(childIndex);
        parseSentence(currentNodeIter);
        currentNodeIter.goToParent();
    }
}

void Parser::parseApplication(ParsingTreeIterator currentNodeIter) const
{
    const auto tokenString = currentNodeIter->getTokenString();
    auto argumentsOffsets = separateArgumentOffsets(currentNodeIter);
    checkMinimumApplicationArgumentNumber(argumentsOffsets, currentNodeIter, tokenString);

    appendArgumentsNodes(argumentsOffsets, currentNodeIter);

    parseArgumentsNodes(currentNodeIter);
}

void Parser::parseSentence(ParsingTreeIterator currentNodeIter) const
{
    const auto tokenString = currentNodeIter->getTokenString();

    if(tokenString.isEmpty())
    {
        throw std::invalid_argument("The string is empty!");
    }

    if(isAtomic(tokenString))
    {
        return;
    }
    else if(hasMolecularForm(tokenString))
    {
        parseApplication(currentNodeIter);
    }
    else
    {
        //When it has neither the atomic or molecular form it must be
        //somehow malformed.
        analyzeError(currentNodeIter);
    }
}

bool Parser::isAtomic(const TokenString &tokenString) const
{
    return tokenString.size() == 1 &&
           tokenString.first().getString() != "(" &&
           tokenString.first().getString() != ")";
}

bool Parser::hasMolecularForm(const TokenString &tokenString) const
{
    return tokenString.first().getString() == "(" &&
           tokenString.last().getString() == ")";
}

bool Parser::isDelimiter(const Token &token) const
{
    return token.getString() == "(" ||
           token.getString() == ")";
}

bool Parser::outermostParenthesisMismatch(const TokenString &tokenString) const
{
    return !(tokenString.first().getString() == "(" &&
           tokenString.last().getString() == ")");
}

QVector<Parser::ArgumentOffsets> Parser::separateArgumentOffsets(ParsingTreeIterator currentNodeIter) const
{
    QVector<ArgumentOffsets> offsets;

    const auto tokenString = currentNodeIter->getTokenString();
    const auto firstDelimiterCompensation = 1;
    const auto lastOffset = tokenString.size() - firstDelimiterCompensation;
    unsigned int argumentBeginOffset = firstDelimiterCompensation;
    unsigned int argumentEndOffset = 0;

    while(argumentBeginOffset < lastOffset)
    {
        if(tokenString[argumentBeginOffset].getString() == ")")
        {
            const unsigned int argumentBeginIndex = currentNodeIter->getBeginIndex() + argumentBeginOffset;
            throw ParsingErrorException<TokenString>("A \")\" was found where a \"(\" or a token was expected!",
                                                     argumentBeginIndex,
                                                     argumentBeginIndex,
                                                     currentNodeIter.goToRoot()->getTokenString());
        }
        else if(tokenString[argumentBeginOffset].getString() == "(")
        {
            try
            {
                argumentEndOffset = ParsingAuxiliaryTools::findDelimiterScopeEndIndex<TokenString>(tokenString,
                                                                                                   PunctuationToken("("),
                                                                                                   PunctuationToken(")"),
                                                                                                   argumentBeginOffset);
            }
            catch(const std::invalid_argument)
            {
                throw ParsingErrorException<TokenString>("There are unmatched parenthesis!",
                                            argumentBeginOffset,
                                            argumentBeginOffset,
                                            currentNodeIter.goToRoot()->getTokenString());
            }
        }
        else
        {
            argumentEndOffset = argumentBeginOffset;
        }


        offsets.push_back(ArgumentOffsets(argumentBeginOffset, argumentEndOffset));
        argumentBeginOffset = argumentEndOffset + 1;
    }

    return offsets;
}

void Parser::performTypeChecking() const
{
    ParsingTreeIterator rootIter(parsingTree.get());

    checkType(rootIter);

    if(rootIter->getType() == wellFormedFormulaType)
    {
        return;
    }
    else
    {
        throw std::invalid_argument((QString("The type of the sentence (")+ rootIter->getType().toString() +") is not the type of well formed formulas (" + wellFormedFormulaType.toString() + "!").toStdString());
    }
}

void Parser::setArgumentsTypes(QVector<Type> &argumentsTypes, ParsingTreeIterator &currentNodeIter) const
{
    for(unsigned int childNumber = 1; childNumber < currentNodeIter->getChildrenNumber(); childNumber++)
    {
        currentNodeIter.goToChild(childNumber);
        checkType(currentNodeIter);
        argumentsTypes.push_back(currentNodeIter->getType());
        currentNodeIter.goToParent();
    }
}

const Type Parser::setMainOperatorType(ParsingTreeIterator iter) const
{
    iter.goToChild(0);
    checkType(iter);
    const Type mainOperatorType(iter->getType());
    iter.goToParent();

    return mainOperatorType;
}

void Parser::checkType(ParsingTreeIterator iter) const
{
    const auto tokenString = iter->getTokenString();

    if(isAtomic(tokenString))
    {
        iter->setType(static_cast<const CoreToken &>(tokenString.first()).getType());
        return;
    }
    else
    {
        QVector<Type> argumentsTypes;
        const auto mainOperatorType = setMainOperatorType(iter);

        setArgumentsTypes(argumentsTypes, iter);

        iter->setType(mainOperatorType.applyArguments(argumentsTypes));
    }
}

void Parser::setVariablesNodes(QVector<QVector<ParsingTreeNode *>> &nodesMatrix) const
{
    for(auto &nodesRow : nodesMatrix)
    {
        for(auto node : nodesRow)
        {
            const TokenString currentNodeTokenString = node->getTokenString();
            if(isVariableToken(currentNodeTokenString))
            {
                node->freeVariables.insert(static_cast<const VariableToken *>(&currentNodeTokenString.first()));
            }
        }
    }
}

void Parser::performVariableBindingChecking() const
{
    auto nodesMatrix = orderNodesByLevel();
    setVariablesNodes(nodesMatrix);

    //We go from the Bottom -> Up (Reverse Iterator)
    //NOTE REFACTOR THIS +1
    std::for_each(nodesMatrix.rbegin() + 1, nodesMatrix.rend(), [this](QVector<ParsingTreeNode *> &nodeRow)
    {
        for(auto parentNode : nodeRow)
        {
            if(nodeHasBindingTokenAtChildren(parentNode))
            {
                performVariableBinding(parentNode);
            }
            propagateFreeAndBoundVariables(parentNode);
        }
    });

    const auto *root = &(*ParsingTreeIterator(parsingTree.get()));
    if(!root->freeVariables.isEmpty())
    {
        throw std::invalid_argument("There are free variables at the top level!");
    }

}

QVector<QVector<ParsingTreeNode *> > Parser::orderNodesByLevel() const
{
    QVector<QVector<ParsingTreeNode *>> nodesMatrix;
    ParsingTreeIterator iter(parsingTree.get());
    auto *root = &(*iter);

    //First Row
    QVector<ParsingTreeNode *> firstRow;
    firstRow.push_back(root);
    nodesMatrix.push_back(firstRow);

    //Remaining Rows
    for(unsigned int currentHeight = 1; currentHeight <= parsingTree->getHeight(); currentHeight++)
    {
        const auto parentRowCompensation = 1;
        QVector<ParsingTreeNode *> currentRow;
        for(auto parentNode : nodesMatrix[currentHeight - parentRowCompensation])
        {
            for(const auto &childNode : parentNode->children)
            {
                currentRow.push_back(childNode.get());
            }
        }
        nodesMatrix.push_back(currentRow);
    }

    return nodesMatrix;
}

bool Parser::isVariableToken(const TokenString &tokenString) const
{
    return tokenString.size() == 1 && tokenString.first().tokenClass() == "VariableToken";
}

bool Parser::nodeHasBindingTokenAtChildren(const ParsingTreeNode *node) const
{
    return !node->children.isEmpty() &&
            node->children.first()->getTokenString().size() == 1 &&
            node->children.first()->getTokenString().first().tokenClass() == "BindingToken";
}

void Parser::performVariableBinding(ParsingTreeNode *parentNode) const
{
    const auto *bindingToken = static_cast<const BindingToken *>(&parentNode->children.first()->getTokenString().first());
    auto &children = parentNode->children;
    auto records = bindingToken->getBindingRecords();  //Remember that iterators are implemented internally as pointers, so if you get the same object two different times by value, their pointers, even tough they point to to equal objects, will be different!

    for(const auto &record : records)
    {
        const auto firstChildIsBindingTokenItselfCompensation = 1;
        const auto bindingArgumentChildIndex = record.bindingArgumentIndex + firstChildIsBindingTokenItselfCompensation;
        if(!isVariableToken(children[static_cast<int>(bindingArgumentChildIndex)]->getTokenString()))
        {
            throw std::invalid_argument("Variable Token expected here!");
        }
        else
        {
            const auto *currentBindingVariable = static_cast<const VariableToken *>(&children[static_cast<int>(bindingArgumentChildIndex)]->getTokenString().first());

            for(const auto boundArgumentIndex : record.boundArgumentsIndexes)
            {
                const unsigned int boundArgumentChildIndex = boundArgumentIndex + firstChildIsBindingTokenItselfCompensation;
                if(children[static_cast<int>(boundArgumentChildIndex)]->boundVariables.contains(currentBindingVariable)) //Care! Ptr comparison!
                {
                    throw std::invalid_argument("Variable is already bound!"); //Variable is already bound!
                }
                else if(!children[static_cast<int>(boundArgumentChildIndex)]->freeVariables.contains(currentBindingVariable))
                {
                    throw std::invalid_argument("This variable doesn't exist in this context!"); //Variable is not free in this context!
                }
                else
                {
                    parentNode->boundVariables.insert(currentBindingVariable);
                }
            }
        }
    }
}

void Parser::propagateFreeAndBoundVariables(ParsingTreeNode *parentNode) const
{
    for(const auto childNode : parentNode->children)
    {
        auto childFreeVariablesCopy = childNode->freeVariables; //So operation doesn't affect the original object
        parentNode->freeVariables.unite(childFreeVariablesCopy.subtract(parentNode->boundVariables));
        parentNode->boundVariables.unite(childNode->boundVariables);

    }
}




















