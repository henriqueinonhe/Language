#ifndef LEXER_H
#define LEXER_H

class Signature;
class TokenString;
class QString;
class QChar;

class Lexer
{
public:
    Lexer() = delete;
    Lexer(const Lexer &) = default;
    Lexer(Lexer &&) noexcept = default;
    Lexer &operator =(const Lexer &) = default;
    Lexer &operator =(Lexer &&) noexcept = default;

    Lexer(const Signature * const signature);

    TokenString lex(const QString &string) const;

    const Signature *getSignature() const;

private:
    bool isSeparator(const QChar &character) const;
    bool stringHasEnded(const QString &string, const int index) const;
    int findTokenBreakpointIndex(const QString &string, int beginIndex) const;
    void lexCoreToken(TokenString &tokenString, int &index, const QString &string) const;

    const Signature *signature;
};

#endif // LEXER_H
