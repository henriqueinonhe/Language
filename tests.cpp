#include "catch.hpp"
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "typeparsingtreenode.h"
#include "type.h"
#include "token.h"
#include "pool.h"
#include "typetokenstring.h"
#include <QRegularExpression>
#include <iostream>
#include "parsingerrorexception.hpp"
#include "tokenstring.h"
#include "lexer.h"
#include "signature.h"
#include "tablesignature.h"
#include "coretoken.h"
#include "punctuationtoken.h"
#include "parsingtree.h"
#include "parsingtreeiterator.h"
#include "parsingtreenode.h"
#include "typeparser.h"
#include "formula.h"
#include "parser.h"

TEST_CASE("TypeParsingTrees")
{
    SECTION("TypeParsingTrees Nodes methods work")
    {
        TypeParsingTree tree(TypeTokenString("Aflosos"));
        TypeParsingTreeIterator iter(&tree);

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->getHeight() == 0);
        CHECK(iter->isRoot());

        iter->appendChild(0,0);
        iter.goToChild(0);

        CHECK(iter->coordinatesToString() == QString("(0)"));
        CHECK(iter->getHeight() == 1);

        iter.goToParent();

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->getHeight() == 0);

        iter->appendChild(0,0);
        iter.goToChild(1);

        CHECK(iter->coordinatesToString() == QString("(1)"));
        CHECK(iter->getHeight() == 1);

        iter->appendChild(0,0);
        iter.goToChild(0);

        CHECK(iter->coordinatesToString() == QString("(1,0)"));
        CHECK(iter->getHeight() == 2);

        iter.goToRoot();

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->isRoot());
    }

    SECTION("Tree methods and Tree Height")
    {
        TypeParsingTree tree(TypeTokenString("Aflosis"));
        TypeParsingTreeIterator iter(&tree);

        CHECK(iter.getTree().getHeight() == 0);

        iter->appendChild(0,0);

        CHECK(iter.getTree().getHeight() == 1);

        iter.goToChild(0);

        iter->appendChild(0,0);

        CHECK(iter.getTree().getHeight() == 2);

        iter.goToParent();

        iter->appendChild(0,0);

        CHECK(iter.getTree().getHeight() == 2);
    }

    SECTION("TypeParsingTree Iterator Paths")
    {
        TypeParsingTree tree(TypeTokenString("Aflisis"));
        TypeParsingTreeIterator iter(&tree);

        iter->appendChild(0,0);
        iter->appendChild(0,0);

        iter.goToChild(1);

        iter->appendChild(0,0);
        iter->appendChild(0,0);

        iter.goToChild(0);

        iter->appendChild(0,0);
        iter->appendChild(0,0);

        iter.goToRoot();
        iter.travelPath("(0)");
        CHECK(iter->coordinatesToString() == QString("(0)"));

        iter.goToRoot();
        iter.travelPath("(1)");
        CHECK(iter->coordinatesToString() == QString("(1)"));

        iter.goToRoot();
        iter.travelPath("(1,0)");
        CHECK(iter->coordinatesToString() == QString("(1,0)"));

        iter.goToRoot();
        iter.travelPath("(1,1)");
        CHECK(iter->coordinatesToString() == QString("(1,1)"));

        iter.goToRoot();
        iter.travelPath("(1,0,0)");
        CHECK(iter->coordinatesToString() == QString("(1,0,0)"));

        iter.goToRoot();
        iter.travelPath("(1,0,1)");
        CHECK(iter->coordinatesToString() == QString("(1,0,1)"));

        CHECK_THROWS(iter.travelPath("(0,0,)"));
        CHECK_THROWS(iter.travelPath("(00,)"));
        CHECK_THROWS(iter.travelPath("(0,0,)2"));
        CHECK_THROWS(iter.travelPath("(0,0,0"));
        CHECK_THROWS(iter.travelPath("0,0"));

        iter.goToCoordinates("(1,0,1)");
    }

    SECTION("Other Methods")
    {
        CHECK(TypeParsingTree(TypeTokenString("Aflosis")) == TypeParsingTree(TypeTokenString("Aflosis")));

        CHECK(TypeParsingTree(TypeTokenString("a")) != TypeParsingTree(TypeTokenString("b")));
    }

}

TEST_CASE("TypeToken")
{
    SECTION("Testing lexing")
    {
        CHECK(TypeToken("(").getSort() == TypeToken::Sort::LeftParenthesis);
        CHECK(TypeToken(")").getSort() == TypeToken::Sort::RightParenthesis);
        CHECK(TypeToken("[").getSort() == TypeToken::Sort::LeftSquareBracket);
        CHECK(TypeToken("]").getSort() == TypeToken::Sort::RightSquareBracket);
        CHECK(TypeToken(",").getSort() == TypeToken::Sort::Comma);
        CHECK(TypeToken("->").getSort() == TypeToken::Sort::CompositionOperator);
        CHECK(TypeToken("Abugabugabugauga").getSort() == TypeToken::Sort::PrimitiveType);

        CHECK(TypeToken().setString("(").getSort() == TypeToken::Sort::LeftParenthesis);
        CHECK(TypeToken().setString(")").getSort() == TypeToken::Sort::RightParenthesis);
        CHECK(TypeToken().setString("[").getSort() == TypeToken::Sort::LeftSquareBracket);
        CHECK(TypeToken().setString("]").getSort() == TypeToken::Sort::RightSquareBracket);
        CHECK(TypeToken().setString(",").getSort() == TypeToken::Sort::Comma);
        CHECK(TypeToken().setString("->").getSort() == TypeToken::Sort::CompositionOperator);
        CHECK(TypeToken().setString("Abugabugabugauga").getSort() == TypeToken::Sort::PrimitiveType);

        CHECK_THROWS(TypeToken("sd-ad"));
        CHECK_THROWS(TypeToken("sd,ad"));
        CHECK_THROWS(TypeToken("sd>ad"));
        CHECK_THROWS(TypeToken("sd(ad"));
        CHECK_THROWS(TypeToken("sd)ad"));
        CHECK_THROWS(TypeToken("sd[ad"));
        CHECK_THROWS(TypeToken("sd]ad"));
        CHECK_THROWS(TypeToken("sd{ad"));
        CHECK_THROWS(TypeToken("sd}ad"));
        CHECK_THROWS(TypeToken(""));
        CHECK_THROWS(TypeToken(" "));

        CHECK_NOTHROW(TypeToken());
    }

    SECTION("Other Tests")
    {
        CHECK(TypeToken("bundinha") == TypeToken("bundinha"));
        CHECK(TypeToken("(") == TypeToken("("));
        CHECK(TypeToken(")") == TypeToken(")"));
        CHECK(TypeToken("[") == TypeToken("["));
        CHECK(TypeToken("]") == TypeToken("]"));
        CHECK(TypeToken("->") == TypeToken("->"));
        CHECK(TypeToken(",") == TypeToken(","));

        CHECK(TypeToken("asdasd") != TypeToken("dsa"));

    }
}

TEST_CASE("Pool")
{
    Pool<int> pool;

    SECTION("")
    {
        PoolRecordPointer<int> ptr;

        ptr = pool.getPointer(1);

        CHECK(*ptr == 1);

        PoolRecordPointer<int> ptr2 = pool.getPointer(1);

        CHECK(*ptr == 1);

        ptr2 = pool.getPointer(2);

        CHECK(*ptr2 == 2);

        PoolRecordPointer<int> ptr3 = pool.getPointer(3);

        CHECK(*ptr3 == 3);

    }

}

TEST_CASE("TypeTokenString")
{   

    TypeTokenString string = TypeTokenString("[i,o]->o");

    CHECK(string.toString() == "[i,o]->o");

    string = TypeTokenString("o->(o->o)");

    CHECK(string.toString() == "o->(o->o)");

    string = TypeTokenString("[o,o]->o");

    CHECK(string.toString() == "[o,o]->o");

    string = TypeTokenString("[Variable,IndividualConstant,PropositionalType]->PropositionalType");

    CHECK(string.toString() == "[Variable,IndividualConstant,PropositionalType]->PropositionalType");

    CHECK(TypeTokenString("[Proposition,Proposition]->Proposition") == TypeTokenString("[Proposition,Proposition]->Proposition"));
    CHECK(TypeTokenString("A->(A->(A->(A->B)))") == TypeTokenString("A->(A->(A->(A->B)))"));

    CHECK(TypeTokenString("A") != TypeTokenString("B"));

    CHECK_THROWS(TypeTokenString(" "));

    CHECK_NOTHROW(TypeTokenString());
    CHECK_NOTHROW(TypeTokenString(""));
}

TEST_CASE("TypeParsingTree Printer") //Re check this whole thing
{
    TypeParsingTree tree(TypeTokenString("[Variable,IndividualConstant,PropositionalType]->PropositionalType"));
    TypeParsingTreeIterator iter(&tree);

    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Composition);
    iter->appendChild(0,10);
    iter->appendChild(12,12);

    iter.goToChild(0);
    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Product);

    iter->appendChild(1,5);
    iter->appendChild(7,9);

    iter.goToChild(0);
    //iter->setMainOperator(TypeParsingTreeNode::MainOperator::Union);  RECHECK THIS

    iter->appendChild(2,2);
    iter->appendChild(4,4);

    iter.goToParent();

    iter.goToChild(1);
    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Primitive);

    iter->appendChild(8,8);

    std::cout << tree.print().toStdString();
}

TEST_CASE("Type")
{
    SECTION("Pass")
    {
        CHECK_NOTHROW(TypeParser::parse("AFLUSISS"));
        CHECK_NOTHROW(TypeParser::parse("Proposition->Proposition"));
        CHECK_NOTHROW(TypeParser::parse("[Proposition,Proposition]->Proposition"));
        CHECK_NOTHROW(TypeParser::parse("[IndividualVariable,IndividualConstant,"
                           "IndividualVariable,IndividualConstant]->Proposition"));
        CHECK_NOTHROW(TypeParser::parse("[IndividualVariable,Proposition]->Proposition"));
        CHECK_NOTHROW(TypeParser::parse("Proposition->(Proposition->Proposition)"));
        CHECK_NOTHROW(TypeParser::parse("(o->o)->o"));
        CHECK_NOTHROW(TypeParser::parse("[o->o,o->o]->o"));
        CHECK_NOTHROW(TypeParser::parse("[a,b,c,d,e,f,g,h,i,j]->k"));
        CHECK_NOTHROW(TypeParser::parse("A->(A->(A->(A->B)))"));
    }

    SECTION("Fail")
    {
        CHECK_THROWS(TypeParser::parse(""));
        CHECK_THROWS(TypeParser::parse("[[o]->o]->o"));
        CHECK_THROWS(TypeParser::parse("[Proposition]->([Proposition]->Proposition)"));
        CHECK_THROWS(TypeParser::parse("[Proposition]->Proposition"));
        CHECK_THROWS(TypeParser::parse("Proposition->([Proposition]->Proposition)"));
        CHECK_THROWS(TypeParser::parse("[o->o]->o"));
        CHECK_THROWS(TypeParser::parse("(o]->o)"));
        CHECK_THROWS(TypeParser::parse("[,]->o"));
        CHECK_THROWS(TypeParser::parse("[o,a)->a"));
        CHECK_THROWS(TypeParser::parse("o->[i,i]"));
        CHECK_THROWS(TypeParser::parse("o->(o]"));
        CHECK_THROWS(TypeParser::parse("o->[i,i]"));
        CHECK_THROWS(TypeParser::parse("[0,1]"));
    }

    SECTION("Other Methods")
    {
        CHECK(TypeParser::parse("o->o") == TypeParser::parse("o->o"));

        CHECK(TypeParser::parse("o") != TypeParser::parse("a->([c,d]->g)"));
    }
}

TEST_CASE("ParsingErrorException")
{
    SECTION("Type Parsing Error Exception")
    {
        TypeTokenString str("[o,o]->[o,o]");
        try
        {
            throw ParsingErrorException<TypeTokenString>("The right hand side argument of the composition operator cannot be a product type!",
                                                         6,
                                                         10,
                                                         str);
        }
        catch(ParsingErrorException<TypeTokenString> &e)
        {
            CHECK(QString(e.what()) == QString("The right hand side argument of the composition operator cannot be a product type!\n"
                              "[o,o]->[o,o]\n"
                              "       ^^^^^"));
        }

        TypeTokenString str2("[Aflisis,Aflosis]->(a->b");
    }

}

TEST_CASE("Punctuation Token")
{
    CHECK_NOTHROW(PunctuationToken("("));
    CHECK_NOTHROW(PunctuationToken(")"));

    CHECK_THROWS(PunctuationToken(","));
    CHECK_THROWS(PunctuationToken(" "));
    CHECK_THROWS(PunctuationToken("Abacateiro"));
    CHECK_THROWS(PunctuationToken("Abacate"));
    CHECK_THROWS(PunctuationToken("( "));
    CHECK_THROWS(PunctuationToken(" )"));
    CHECK_THROWS(PunctuationToken(" , "));
    CHECK_THROWS(PunctuationToken("(("));
    CHECK_THROWS(PunctuationToken("(,)"));
}

TEST_CASE("Core Token")
{
    Type a(TypeParser::parse("o"));

    CHECK_NOTHROW(CoreToken("asdsad", a));
    CHECK_NOTHROW(CoreToken("Abacate", a));
    CHECK_NOTHROW(CoreToken("Afleflesis", a));
    CHECK_NOTHROW(CoreToken("Afleisis", a));

    CHECK_THROWS(CoreToken("(", a));
    CHECK_THROWS(CoreToken(")", a));
    CHECK_THROWS(CoreToken(",", a));
    CHECK_THROWS(CoreToken("das sdsd", a));
    CHECK_THROWS(CoreToken("asdsd,dsds", a));
    CHECK_THROWS(CoreToken("IASD(dds", a));
}

TEST_CASE("Lexer, Table Signature and Type Token String")
{
    TableSignature signature;

    //Setting up Signature
    PunctuationToken t1("(");
    PunctuationToken t2(")");

    signature.addToken(&t1);
    signature.addToken(&t2);

    SECTION("Token pointers work")
    {
        CHECK(signature.getTokenPointer("(") == signature.getTokenPointer("("));
        CHECK(signature.getTokenPointer(")") == signature.getTokenPointer(")"));

        CHECK_THROWS(signature.getTokenPointer("Aflisis"));
        CHECK_THROWS(signature.getTokenPointer("P"));
        CHECK_THROWS(signature.getTokenPointer("Int"));

        CHECK_THROWS(signature.addToken(&t1));
        CHECK_THROWS(signature.addToken(&t2));
    }

    //Some Core Tokens
    Type type1 = TypeParser::parse("i->o");
    Type type2 = TypeParser::parse("i");

    CoreToken t4("P", type1);
    CoreToken t5("a", type2);

    signature.addToken(&t4);
    signature.addToken(&t5);

    //Setting Up Lexer
    Lexer lexer(&signature);

    SECTION("Lexing strings")
    {
        SECTION("Tokens that haven't been declared")
        {
            CHECK_THROWS(lexer.lex("b"));
            CHECK_THROWS(lexer.lex("c"));
        }

        SECTION("Lexing is working properly")
        {
            CHECK(lexer.lex("(P a)").formattedString() == QString("(P a)"));
            CHECK(lexer.lex("(a P)").formattedString() == QString("(a P)"));
        }

        SECTION("Whitespace is handled properly")
        {
            CHECK(lexer.lex("P        a()").formattedString() == QString("P a ()"));
            CHECK(lexer.lex("(P a a a a P         P  ) ()()()").formattedString() == QString("(P a a a a P P) () () ()"));
        }
    }
}

TEST_CASE("ParsingTrees")
{
    Type type = TypeParser::parse("i");
    CoreToken token("A", type);

    TableSignature signature;
    signature.addToken(&token);

    Lexer lexer(&signature);

    SECTION("ParsingTrees Nodes methods work")
    {
        ParsingTree tree(lexer.lex("A"));
        ParsingTreeIterator iter(&tree);

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->getHeight() == 0);
        CHECK(iter->isRoot());

        iter->appendChild(0,0);
        iter.goToChild(0);

        CHECK(iter->coordinatesToString() == QString("(0)"));
        CHECK(iter->getHeight() == 1);

        iter.goToParent();

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->getHeight() == 0);

        iter->appendChild(0,0);
        iter.goToChild(1);

        CHECK(iter->coordinatesToString() == QString("(1)"));
        CHECK(iter->getHeight() == 1);

        iter->appendChild(0,0);
        iter.goToChild(0);

        CHECK(iter->coordinatesToString() == QString("(1,0)"));
        CHECK(iter->getHeight() == 2);

        iter.goToRoot();

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->isRoot());
    }

    SECTION("Tree methods and Tree Height")
    {
        ParsingTree tree(lexer.lex("A"));
        ParsingTreeIterator iter(&tree);

        CHECK(iter.getTree().getHeight() == 0);

        iter->appendChild(0,0);

        CHECK(iter.getTree().getHeight() == 1);

        iter.goToChild(0);

        iter->appendChild(0,0);

        CHECK(iter.getTree().getHeight() == 2);

        iter.goToParent();

        iter->appendChild(0,0);

        CHECK(iter.getTree().getHeight() == 2);
    }

    SECTION("ParsingTree Iterator Paths")
    {
        ParsingTree tree(lexer.lex("A"));
        ParsingTreeIterator iter(&tree);

        iter->appendChild(0,0);
        iter->appendChild(0,0);

        iter.goToChild(1);

        iter->appendChild(0,0);
        iter->appendChild(0,0);

        iter.goToChild(0);

        iter->appendChild(0,0);
        iter->appendChild(0,0);

        iter.goToRoot();
        iter.travelPath("(0)");
        CHECK(iter->coordinatesToString() == QString("(0)"));

        iter.goToRoot();
        iter.travelPath("(1)");
        CHECK(iter->coordinatesToString() == QString("(1)"));

        iter.goToRoot();
        iter.travelPath("(1,0)");
        CHECK(iter->coordinatesToString() == QString("(1,0)"));

        iter.goToRoot();
        iter.travelPath("(1,1)");
        CHECK(iter->coordinatesToString() == QString("(1,1)"));

        iter.goToRoot();
        iter.travelPath("(1,0,0)");
        CHECK(iter->coordinatesToString() == QString("(1,0,0)"));

        iter.goToRoot();
        iter.travelPath("(1,0,1)");
        CHECK(iter->coordinatesToString() == QString("(1,0,1)"));

        CHECK_THROWS(iter.travelPath("(0,0,)"));
        CHECK_THROWS(iter.travelPath("(00,)"));
        CHECK_THROWS(iter.travelPath("(0,0,)2"));
        CHECK_THROWS(iter.travelPath("(0,0,0"));
        CHECK_THROWS(iter.travelPath("0,0"));

        iter.goToCoordinates("(1,0,1)");
    }
}

TEST_CASE("Parser")
{
    //SETUP
    TableSignature signature;

    PunctuationToken leftP("(");
    PunctuationToken rightP(")");

    CoreToken t1("P", TypeParser::parse("o")),
              t2("Q", TypeParser::parse("o")),
              t3("And", TypeParser::parse("[o,o]->o")),
              t4("Not", TypeParser::parse("o->o")),
              t5("Implies", TypeParser::parse("[o,o]->o")),
              t6("Or", TypeParser::parse("[o,o]->o")),
              t7("Equivalent", TypeParser::parse("[o,o]->o"));

    signature.addToken(&leftP);
    signature.addToken(&rightP);
    signature.addToken(&t1);
    signature.addToken(&t2);
    signature.addToken(&t3);
    signature.addToken(&t4);
    signature.addToken(&t5);
    signature.addToken(&t6);
    signature.addToken(&t7);

    Parser parser(&signature, TypeParser::parse("o"));

    //END SETUP

    SECTION("Pass")
    {
        CHECK_NOTHROW(parser.parse("P"));
        CHECK_NOTHROW(parser.parse("Q"));
        CHECK_NOTHROW(parser.parse("(And P Q)"));
        CHECK_NOTHROW(parser.parse("(And Q P)"));
        CHECK_NOTHROW(parser.parse("(Not P)"));
        CHECK_NOTHROW(parser.parse("(Not Q)"));
        CHECK_NOTHROW(parser.parse("(And (Not Q) (Not P))"));

        CHECK_NOTHROW(parser.parse("(Equivalent (And P Q) (And Q P))"));
        CHECK_NOTHROW(parser.parse("(Implies (And P Q) P)"));
        CHECK_NOTHROW(parser.parse("(Implies (And P Q) Q)"));
        CHECK_NOTHROW(parser.parse("(Implies (And (Or P Q) (Not Q) ) P)"));
        CHECK_NOTHROW(parser.parse("(Implies (And (Or P Q) (Not P) ) Q)"));
        CHECK_NOTHROW(parser.parse("(Implies (And (Implies P Q) P)  Q )"));
    }

    SECTION("Fail Due To Structure")
    {
        CHECK_THROWS(parser.parse(""));
        CHECK_THROWS(parser.parse("("));
        CHECK_THROWS(parser.parse("()"));
        CHECK_THROWS(parser.parse("F"));
        CHECK_THROWS(parser.parse("AndP"));
        CHECK_THROWS(parser.parse("PQ"));
        CHECK_THROWS(parser.parse("(P)"));
        CHECK_THROWS(parser.parse("(())"));
        CHECK_THROWS(parser.parse("Not P"));
        CHECK_THROWS(parser.parse("((Not P))"));
        CHECK_THROWS(parser.parse("(Not (P))"));
        CHECK_THROWS(parser.parse(")Not P("));
    }

    SECTION("Fail Due To Type Checking")
    {
        CHECK_THROWS(parser.parse("(Not (And P Q) P)"));
    }

}
