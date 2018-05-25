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

QString BasicPreProcessor::processString(QString string) const
{
    //Here begins the real fun!

    //1. Lexing
    TokenString tokenString = lexer.lex(string);

    //2. Auxiliary Vector
    QVector<EnhancedRecord> necessaryRecords;

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

    //3. Processing
    std::for_each(necessaryRecords.begin(), necessaryRecords.end(), [&tokenString](const EnhancedRecord &record)
    {
        if(record.record->associativity == BasicProcessorTokenRecord::Associativity::Left)
        {
            for(unsigned int tokenIndex = 0; tokenIndex < tokenString.size(); tokenIndex++)
            {
                if(tokenString[tokenIndex].getString() == record.record->token)
                {
                    //Commence rearranging
                    const unsigned int tokenLookaheadCompensation = 1;

                    //Count Backwards
                    const unsigned int numberOfArgumentsBeforeOperator = record.record->operatorPosition;
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
                    const unsigned int leftParenthesisIndex = backwardsScannerIndex;


                    //Count Afterwards
                    const unsigned int numberOfArgumentsAfterOperator = record.record->numberOfArguments - record.record->operatorPosition;
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
                            afterwardsScannerIndex = ParsingAuxiliaryTools::findDelimiterScopeEndIndex(tokenString,
                                                                                                       PunctuationToken("("),
                                                                                                       PunctuationToken(")"),
                                                                                                       afterwardsScannerIndex,
                                                                                                       ParsingAuxiliaryTools::ParsingDirection::LeftToRight);
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
                    const unsigned int rightParenthesisIndex = afterwardsScannerIndex;

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
                    //Commence rearranging
                }
            }
        }
        else
        {
            throw std::logic_error("This shouldn't be happening!");
        }
    });

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
