﻿#include "parser.h"

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

    //TODO Cache stuff and return formula and type stuff
    buildParsingTree(sentence);

    performTypeChecking();

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

void Parser::performVariableBindingChecking()
{
    QVector<QVector<ParsingTreeNode *>> nodesMatrix;
    ParsingTreeIterator iter(parsingTree.get());
    ParsingTreeNode *root = &(*iter);

    //Laying down nodes ordered by level!
    const unsigned int startAtBeforeLastRowCompensation = 1;
    for(unsigned int currentHeight = 0; currentHeight < parsingTree->height - startAtBeforeLastRowCompensation; currentHeight++)
    {
        QVector<ParsingTreeNode *> nodesRow;
        std::for_each(root->children.begin(),
                      root->children.end(),
                      [&nodesRow](shared_ptr<ParsingTreeNode> node)
        {
            nodesRow.push_back(node.get());
        });
        nodesMatrix.push_back(nodesRow);
    }


}



















