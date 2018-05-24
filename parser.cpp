#include "parser.h"

Parser::Parser(Signature *signature, const Type &wellFormedFormulaType) :
    lexer(signature),
    wellFormedFormulaType(wellFormedFormulaType)
{
    if(signature == nullptr)
    {
        throw std::invalid_argument("Signature pointer is null!");
    }
}

Formula Parser::parse(const QString &sentence)
{

    //TODO Cache stuff
    buildParsingTree(sentence);

    performTypeChecking();

    performVariableBindingChecking();

    std::cout << parsingTree->print().toStdString();

    return Formula(lexer.lex(sentence));
}

void Parser::buildParsingTree(const QString &sentence)
{
    const TokenString tokenString(lexer.lex(sentence));

    parsingTree.reset(new ParsingTree(tokenString));

    ParsingTreeIterator iter(parsingTree.get());

    parseSentence(iter);
}

void Parser::analyzeError(ParsingTreeIterator iter)
{
    const TokenString tokenString = iter->getTokenString();
    const unsigned int beginIndex = iter->getBeginIndex();

    if(tokenString.first().getString() == ")")
    {
        throw ParsingErrorException<TokenString>("The first character cannot be a left parenthesis.",
                                                 beginIndex,
                                                 beginIndex,
                                                 iter.goToRoot()->getTokenString());
    }
    else if(outermostParenthesisMismatch(tokenString))
    {
        const unsigned int zeroIndexCompensation = 1;
        const unsigned int endIndex = beginIndex + tokenString.size() - zeroIndexCompensation;
        throw ParsingErrorException<TokenString>("The outermost parenthesis doesn't match!",
                                                 beginIndex,
                                                 endIndex,
                                                 iter.goToRoot()->getTokenString());
    }
    else if(!isDelimiter(tokenString.first()) && tokenString.size() != 1)
    {
        const unsigned int zeroIndexCompensation = 1;
        const unsigned int endIndex = beginIndex + tokenString.size() - zeroIndexCompensation;
        throw ParsingErrorException<TokenString>("A string with more than one token should be an application, which uses parenthesis!",
                                                 beginIndex,
                                                 endIndex,
                                                 iter.goToRoot()->getTokenString());
    }
    else
    {
        throw std::logic_error("This case shouldn't happen in parsing!");
    }
}

void Parser::parseApplication(ParsingTreeIterator iter)
{
    const TokenString tokenString = iter->getTokenString();
    QVector<ArgumentOffsets> offsets = separateArgumentOffsets(iter);

    if(offsets.size() < 2)
    {
        const unsigned int beginIndex = iter->getBeginIndex();
        const unsigned int endIndex = iter->getBeginIndex() + tokenString.size();

        throw ParsingErrorException<TokenString>("An application should have at least two arguments!",
                                                 beginIndex,
                                                 endIndex,
                                                 tokenString);
    }

    std::for_each(offsets.begin(), offsets.end(), [&iter](const ArgumentOffsets &element)
    {
        const unsigned int argumentBeginIndex = iter->getBeginIndex() + element.beginOffset;
        const unsigned int argumentEndIndex = iter->getBeginIndex() + element.endOffset;

        iter->appendChild(argumentBeginIndex, argumentEndIndex);
    });

    for(unsigned int childIndex = 0; childIndex < iter->getChildrenNumber(); childIndex++)
    {
        iter.goToChild(childIndex);
        parseSentence(iter);
        iter.goToParent();
    }
}

void Parser::parseSentence(ParsingTreeIterator iter)
{
    const TokenString tokenString = iter->getTokenString();

    if(tokenString.isEmpty())
    {
        std::invalid_argument("The string is empty!");
    }

    if(isAtomic(tokenString))
    {
        return;
    }
    else if(hasMolecularForm(tokenString))
    {
        parseApplication(iter);
    }
    else
    {
        analyzeError(iter);
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
    return tokenString.first().getString() == "(" &&
           tokenString.last().getString() == ")";
}

QVector<Parser::ArgumentOffsets> Parser::separateArgumentOffsets(ParsingTreeIterator iter) const
{
    QVector<ArgumentOffsets> offsets;

    const TokenString tokenString = iter->getTokenString();
    const unsigned int firstDelimiterCompensation = 1;
    const unsigned int lastOffset = tokenString.size() - firstDelimiterCompensation;
    unsigned int argumentBeginOffset = firstDelimiterCompensation;
    unsigned int argumentEndOffset = argumentBeginOffset;

    while(argumentBeginOffset < lastOffset)
    {
        if(tokenString[argumentBeginOffset].getString() == ")")
        {
            const unsigned int argumentBeginIndex = iter->getBeginIndex() + argumentBeginOffset;
            throw ParsingErrorException<TokenString>("A \")\" was found where a \"(\" or a token was expected!",
                                                     argumentBeginIndex,
                                                     argumentBeginIndex,
                                                     iter.goToRoot()->getTokenString());
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
                                            iter.goToRoot()->getTokenString());
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

void Parser::performTypeChecking()
{
    ParsingTreeIterator iter(parsingTree.get());

    checkType(iter);

    if(iter->getType() == wellFormedFormulaType)
    {
        return;
    }
    else
    {
        //FIXME Find a more suitable way to handle this! Maybe with a specialized exception
        throw std::invalid_argument("The type of the sentence is not the type of well formed formulas!");
    }
}

void Parser::setArgumentsTypes(QVector<TypeTokenString> &argumentsTypes, ParsingTreeIterator &iter)
{
    for(unsigned int childNumber = 1; childNumber < iter->getChildrenNumber(); childNumber++)
    {
        iter.goToChild(childNumber);
        checkType(iter);
        argumentsTypes.push_back(iter->getType().getTypeString());
        iter.goToParent();
    }
}

const Type Parser::setMainOperatorType(ParsingTreeIterator iter)
{
    iter.goToChild(0);
    checkType(iter);
    const Type mainOperatorType(iter->getType());
    iter.goToParent();

    return mainOperatorType;
}

void Parser::checkType(ParsingTreeIterator iter)
{
    const TokenString tokenString = iter->getTokenString();

    if(isAtomic(tokenString))
    {
        iter->setType(dynamic_cast<CoreToken &>(tokenString.first()).getType());
        return;
    }
    else
    {
        QVector<TypeTokenString> argumentsTypes;
        const Type mainOperatorType = setMainOperatorType(iter);

        setArgumentsTypes(argumentsTypes, iter);

        iter->setType(mainOperatorType.checkType(argumentsTypes));
    }
}

void Parser::setVariablesNodes(QVector<QVector<ParsingTreeNode *>> &nodesMatrix)
{
    std::for_each(nodesMatrix.begin(), nodesMatrix.end(), [this](QVector<ParsingTreeNode *> &nodesRow)
    {
        std::for_each(nodesRow.begin(), nodesRow.end(), [this](ParsingTreeNode *node)
        {
            const TokenString currentNodeTokenString = node->getTokenString();
            if(isVariableToken(currentNodeTokenString))
            {
                node->freeVariables.insert(dynamic_cast<VariableToken *>(&currentNodeTokenString.first()));
            }
        });
    });
}

void Parser::performVariableBindingChecking()
{
    QVector<QVector<ParsingTreeNode *>> nodesMatrix = orderNodesByLevel();
    setVariablesNodes(nodesMatrix);

    //We go from the Bottom -> Up (Reverse Iterator)
    //FIXME REFACTOR THIS +1
    std::for_each(nodesMatrix.rbegin() + 1, nodesMatrix.rend(), [this](QVector<ParsingTreeNode *> &nodeRow)
    {
        std::for_each(nodeRow.begin(), nodeRow.end(), [this](ParsingTreeNode *parentNode)
        {
            if(nodeHasBindingTokenAtChildren(parentNode))
            {
                performVariableBinding(parentNode);
            }
            propagateFreeAndBoundVariables(parentNode);
        });
    });

    const ParsingTreeNode *root = &(*ParsingTreeIterator(parsingTree.get()));

    if(!root->freeVariables.isEmpty())
    {
        throw std::invalid_argument("There are free variables at the top level!");
    }

}

QVector<QVector<ParsingTreeNode *> > Parser::orderNodesByLevel() const
{
    QVector<QVector<ParsingTreeNode *>> nodesMatrix;
    ParsingTreeIterator iter(parsingTree.get());
    ParsingTreeNode *root = &(*iter);

    //First Row
    QVector<ParsingTreeNode *> firstRow;
    firstRow.push_back(root);
    nodesMatrix.push_back(firstRow);

    //Remaining Rows
    for(unsigned int currentHeight = 1; currentHeight <= parsingTree->getHeight(); currentHeight++)
    {
        const unsigned int parentRowCompensation = 1;
        QVector<ParsingTreeNode *> currentRow;
        std::for_each(nodesMatrix[currentHeight - parentRowCompensation].begin(),
                      nodesMatrix[currentHeight - parentRowCompensation].end(),
                      [&currentRow](ParsingTreeNode *parentNode)
        {
            std::for_each(parentNode->children.begin(),
                          parentNode->children.end(),
                          [&currentRow](const shared_ptr<ParsingTreeNode> &childNode)
            {
                currentRow.push_back(childNode.get());
            });
        });
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

void Parser::performVariableBinding(ParsingTreeNode *parentNode)
{
    const BindingToken *bindingToken = dynamic_cast<BindingToken *>(&parentNode->children.first()->getTokenString().first());
    QVector<std::shared_ptr<ParsingTreeNode>> &children = parentNode->children;
    QVector<BindingToken::BindingRecord> records = bindingToken->getBindingRecords();  //Remember that iterators are implemented internally as pointers, so if you get the same object two different times by value, their pointers, even tough they point to to equal objects, will be different!

    std::for_each(records.begin(),
                  records.end(),
                  [&parentNode, &children, this](const BindingToken::BindingRecord &record)
    {
        const unsigned int firstChildIsBindingTokenItselfCompensation = 1;
        const unsigned int bindingArgumentChildIndex = record.bindingArgumentIndex + firstChildIsBindingTokenItselfCompensation;
        if(!isVariableToken(children[bindingArgumentChildIndex]->getTokenString()))
        {
            throw std::invalid_argument("Variable Token expected here!");
        }
        else
        {
            VariableToken *currentBindingVariable = dynamic_cast<VariableToken *>(&children[bindingArgumentChildIndex]->getTokenString().first());

            std::for_each(record.boundArgumentsIndexes.begin(),
                          record.boundArgumentsIndexes.end(),
                          [&currentBindingVariable, &parentNode, &children](const unsigned int boundArgumentIndex)
            {
                const unsigned int boundArgumentChildIndex = boundArgumentIndex + firstChildIsBindingTokenItselfCompensation;
                if(children[boundArgumentChildIndex]->boundVariables.contains(currentBindingVariable)) //Care! Ptr comparison!
                {
                    throw std::invalid_argument("Variable is already bound!"); //Variable is already bound!
                }
                else if(!children[boundArgumentChildIndex]->freeVariables.contains(currentBindingVariable))
                {
                    throw std::invalid_argument("This variable doesn't exist in this context!"); //Variable is not free in this context!
                }
                else
                {
                    parentNode->boundVariables.insert(currentBindingVariable);
                }
            });
        }
    });
}

void Parser::propagateFreeAndBoundVariables(ParsingTreeNode *parentNode)
{
    std::for_each(parentNode->children.begin(), parentNode->children.end(), [&parentNode](std::shared_ptr<ParsingTreeNode> childNode)
    {
        QSet<VariableToken *> childFreeVariablesCopy = childNode->freeVariables; //So operation doesn't affect the original object

        parentNode->freeVariables.unite(childFreeVariablesCopy.subtract(parentNode->boundVariables));
        parentNode->boundVariables.unite(childNode->boundVariables);
    });
}




















