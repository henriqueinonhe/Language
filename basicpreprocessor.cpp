#include "basicpreprocessor.h"


BasicPreProcessor::BasicPreProcessor(Signature * const signature) :
    BasicProcessor(signature)
{

}

void BasicPreProcessor::considerToken(const Token &currentToken, QVector<EnhancedRecord> &necessaryRecords) const
{
    unsigned int currentTokenPrecedenceRank = 0;
    for(auto iter = tokenRecords.begin(); iter != tokenRecords.end(); iter++, currentTokenPrecedenceRank++)
    {
        if(currentToken.getString() == iter->token)
        {
            necessaryRecords.push_back(EnhancedRecord(const_cast<BasicProcessorTokenRecord *>(&(*iter)), currentTokenPrecedenceRank));
        }
    }
}

void BasicPreProcessor::setupNecessaryRecords(const TokenString &tokenString, QVector<EnhancedRecord> &necessaryRecords) const
{
    for(unsigned int tokenIndex = 0; tokenIndex < tokenString.size(); tokenIndex++)
    {
        const Token &currentToken = tokenString[tokenIndex];
        if(tokenAlreadyConsidered(currentToken, necessaryRecords))
        {
            continue;
        }
        else
        {
            considerToken(currentToken, necessaryRecords);
        }
    }

    std::sort(necessaryRecords.begin(), necessaryRecords.end(), [](const EnhancedRecord &record1, const EnhancedRecord &record2)
    {
        return record1.precedenceRank < record2.precedenceRank;
    });
}

unsigned int BasicPreProcessor::findOperatorLeftParenthesisIndex(const TokenString &tokenString, const unsigned int numberOfArgumentsBeforeOperator, const unsigned int tokenIndex) const
{
    const unsigned int tokenLookaheadCompensation = 1;
    unsigned int argumentsBeforeOperatorCount = 0;
    unsigned int backwardsScannerIndex = tokenIndex - tokenLookaheadCompensation;
    while(argumentsBeforeOperatorCount != numberOfArgumentsBeforeOperator)
    {
        if(!tokenString.indexIsWithinBounds(backwardsScannerIndex))
        {
            throw std::invalid_argument("There are arguments missing!"); //FIXME Needs Specialized Exception!
        }

        if(tokenString[backwardsScannerIndex].getString() == ")")
        {
            //Parenthesis Analysis
            try
            {
                backwardsScannerIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                          PunctuationToken("("),
                                                                                          PunctuationToken(")"),
                                                                                          backwardsScannerIndex,
                                                                                          ParsingAuxiliaryTools::ParsingDirection::RightToLeft);
                backwardsScannerIndex++;
            }
            catch(const std::invalid_argument &)
            {
                throw std::invalid_argument("Parenthesis doesn't match!"); //FIXME Needs Specialized Exception!
            }

        }
        else if(tokenString[backwardsScannerIndex].getString() == "(")
        {
            throw std::invalid_argument("Encountered an unexpected '('!"); //FIXME Needs Specialized Exception!
        }
        else
        {
            backwardsScannerIndex--;
        }
        argumentsBeforeOperatorCount++;
    }

    return backwardsScannerIndex + tokenLookaheadCompensation;
}

unsigned int BasicPreProcessor::findOperatorRightParenthesisIndex(const TokenString &tokenString, const unsigned int numberOfArgumentsAfterOperator, unsigned int tokenIndex) const
{
    const unsigned int tokenLookaheadCompensation = 1;
    unsigned int argumentsAfterOperatorCount = 0;
    unsigned int afterwardsScannerIndex = tokenIndex + tokenLookaheadCompensation;

    while(argumentsAfterOperatorCount != numberOfArgumentsAfterOperator)
    {
        if(!tokenString.indexIsWithinBounds(afterwardsScannerIndex))
        {
            throw std::invalid_argument("Index is out of bounds!"); //FIXME Needs Specialized Exception!
        }

        if(tokenString[afterwardsScannerIndex].getString() == "(")
        {
            //Parenthesis Analysis
            try
            {
                afterwardsScannerIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                           PunctuationToken("("),
                                                                                           PunctuationToken(")"),
                                                                                           afterwardsScannerIndex,
                                                                                           ParsingAuxiliaryTools::ParsingDirection::LeftToRight);
            }
            catch(const std::invalid_argument&)
            {
                throw std::invalid_argument("Parenthesis doesn't match!");
            }
            afterwardsScannerIndex++;
        }
        else if(tokenString[afterwardsScannerIndex].getString() == ")")
        {
            throw std::invalid_argument("Encountered an unexpected ')'!"); //FIXME Needs Specialized Exception!
        }
        else
        {
            afterwardsScannerIndex++;
        }
        argumentsAfterOperatorCount++;
    }

    //const unsigned int insertIndexCompensation = 1;
    return afterwardsScannerIndex/* + insertIndexCompensation*/;
}

bool BasicPreProcessor::operatorParenthesisAreAlreadyPlaced(const TokenString &tokenString, const unsigned int leftParenthesisIndex, const unsigned int rightParenthesisIndex) const
{
    bool leftParenthesisIndexIsStringStart = leftParenthesisIndex == 0;
    bool rightParenthesisIndexIsStringEnd = rightParenthesisIndex == tokenString.size();

    if(leftParenthesisIndexIsStringStart || rightParenthesisIndexIsStringEnd)
    {
        return false;
    }
    else
    {
        const unsigned int tokenLookaheadCompensation = 1;
        if(tokenString[leftParenthesisIndex - tokenLookaheadCompensation].getString() == "(" &&
           tokenString[rightParenthesisIndex +  tokenLookaheadCompensation].getString() == ")")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void BasicPreProcessor::insertOperatorParenthesis(TokenString &tokenString, const unsigned int rightParenthesisInsertIndex, const unsigned int leftParenthesisInsertIndex) const
{
    tokenString.insert(leftParenthesisInsertIndex, "(");
    tokenString.insert(rightParenthesisInsertIndex, ")");
}

void BasicPreProcessor::moveOperator(TokenString &tokenString, const unsigned int leftParenthesisInsertIndex, const unsigned int tokenIndex) const
{
    const unsigned int insertedTokenCompensation = 1;
    const unsigned int firstArgumentIndex = leftParenthesisInsertIndex + insertedTokenCompensation;
    tokenString.swapTokens(tokenIndex, firstArgumentIndex);
}

void BasicPreProcessor::processToken(TokenString &tokenString, const unsigned int tokenIndex, const EnhancedRecord &record) const
{
    //Backwards Scanning
    const unsigned int numberOfArgumentsBeforeOperator = record.record->operatorPosition;
    const unsigned int leftParenthesisInsertIndex = findOperatorLeftParenthesisIndex(tokenString, numberOfArgumentsBeforeOperator, tokenIndex);

    //Afterwards Scanning
    const unsigned int numberOfArgumentsAfterOperator = record.record->numberOfArguments - record.record->operatorPosition;
    const unsigned int rightParenthesisInsertIndex = findOperatorRightParenthesisIndex(tokenString, numberOfArgumentsAfterOperator, tokenIndex);

    if(!operatorParenthesisAreAlreadyPlaced(tokenString, leftParenthesisInsertIndex, rightParenthesisInsertIndex))
    {
        insertOperatorParenthesis(tokenString, rightParenthesisInsertIndex, leftParenthesisInsertIndex);
    }

    //Put operator on the right place
    moveOperator(tokenString, leftParenthesisInsertIndex, tokenIndex);
}

QString BasicPreProcessor::processString(QString string) const
{
    //Here begins the real fun!

    //1. Lexing
    TokenString tokenString = Lexer(signature).lex(string);

    //2. Auxiliary Vector
    QVector<EnhancedRecord> necessaryRecords;
    setupNecessaryRecords(tokenString, necessaryRecords);

    //3. Processing
    //Da pra otimizar isso se eu guardar a informação de onde estão os indexes dos tokens que vão ser processados!
    //Mais pra frente vou querer fazer isso pra otimizar o processo, isso tem que ser guardado no enhanced record! Num vetor de unsigned ints!

    std::for_each(necessaryRecords.begin(), necessaryRecords.end(), [&tokenString, this](const EnhancedRecord &record)
    {
        if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Left)
        {
            for(unsigned int tokenIndex = 0; tokenIndex < tokenString.size(); tokenIndex++)
            {
                if(tokenString[tokenIndex].getString() == record.record->token)
                {
                    processToken(tokenString, tokenIndex, record);
                }
            }
        }
        else if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Right)
        {
            const unsigned int zeroIndexCompensation = 1;
            for(int tokenIndex = tokenString.size() - zeroIndexCompensation; tokenIndex >= 0; tokenIndex--)
            {
                if(tokenString[tokenIndex].getString() == record.record->token)
                {
                    processToken(tokenString, tokenIndex, record);
                }
            }
        }
        else
        {
            throw std::logic_error("This shouldn't be happening!");
        }
    });

    return tokenString.toString();
}

QString BasicPreProcessor::toString() const
{
    return "Basic PreProcessor (Operator position, precendece and associativity)";
}

bool BasicPreProcessor::tokenAlreadyConsidered(const Token &token, const QVector<EnhancedRecord> &necessaryRecords) const
{
    return std::any_of(necessaryRecords.begin(), necessaryRecords.end(), [&token](const EnhancedRecord &record)
    {
        return token.getString() == record.record->token;
    });
}
