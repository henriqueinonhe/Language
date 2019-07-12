#include "catch.hpp"
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "typeparsingtreenode.h"
#include "type.h"
#include "token.h"
#include "pool.h"
#include "typetoken.h"
#include "typetokenstring.h"
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
#include "containerauxiliarytools.h"
#include "variabletoken.h"
#include "bindingtoken.h"
#include "basicpreprocessor.h"
#include "stringprocessor.h"
#include "formatter.h"
#include "basicpostprocessor.h"
#include "formula.h"
#include <QFile>
#include <QDataStream>
#include "dirtyfix.h"
#include "bindingrecord.h"
#include <QBuffer>

TEST_CASE("TypeToken")
{
    //Character Admissibiity (Input Validation)
    CHECK(TypeToken("(").sort() == TypeToken::Sort::LeftParenthesis);
    CHECK(TypeToken(")").sort() == TypeToken::Sort::RightParenthesis);
    CHECK(TypeToken("[").sort() == TypeToken::Sort::LeftSquareBracket);
    CHECK(TypeToken("]").sort() == TypeToken::Sort::RightSquareBracket);
    CHECK(TypeToken(",").sort() == TypeToken::Sort::Comma);
    CHECK(TypeToken("->").sort() == TypeToken::Sort::CompositionOperator);
    CHECK(TypeToken("Abugabugabugauga").sort() == TypeToken::Sort::PrimitiveType);

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

    //Equality and Inequality
    CHECK(TypeToken("bundinha") == TypeToken("bundinha"));
    CHECK(TypeToken("(") == TypeToken("("));
    CHECK(TypeToken(")") == TypeToken(")"));
    CHECK(TypeToken("[") == TypeToken("["));
    CHECK(TypeToken("]") == TypeToken("]"));
    CHECK(TypeToken("->") == TypeToken("->"));
    CHECK(TypeToken(",") == TypeToken(","));

    CHECK(TypeToken("asdasd") != TypeToken("dsa"));
}

TEST_CASE("Pool")
{
    class IntWrapper
    {
        public:
        IntWrapper() = default;
        IntWrapper(const int a) : a(a){}
        int a;

        bool operator==(const int value) const
        {
            return this->a == value;
        }

        QString getString() const
        {
            return QString::number(a);
        }
    };

    Pool<IntWrapper> pool;
    PoolRecordPointer<IntWrapper> ptr1, ptr2, ptr3;

    ptr1 = pool.getPointer(1);
    ptr2 = pool.getPointer(2);
    ptr3 = pool.getPointer(1);

    CHECK(*ptr1 == 1);
    CHECK(*ptr2 == 2);
    CHECK(*ptr3 == 1);

    auto iter = pool.getRecords().find("1");

    CHECK(iter->second.getObject() == 1);
    CHECK(iter->second.getCounter() == 2);

    iter = pool.getRecords().find("2");
    CHECK(iter->second.getObject() == 2);
    CHECK(iter->second.getCounter() == 1);

    ptr3 = ptr2;

    iter = pool.getRecords().find("1");
    CHECK(iter->second.getObject() == 1);
    CHECK(iter->second.getCounter() == 1);

    iter = pool.getRecords().find("2");
    CHECK(iter->second.getObject() == 2);
    CHECK(iter->second.getCounter() == 2);
}

TEST_CASE("TypeTokenString")
{
    //Lexing and String Conversion
    CHECK_THROWS(TypeTokenString(" "));
    CHECK_THROWS(TypeTokenString("i-"));
    CHECK_THROWS(TypeTokenString("a>b"));

    CHECK_NOTHROW(TypeTokenString(""));

    TypeTokenString string = TypeTokenString("[i,o]->o");

    CHECK(string.toString() == "[i,o]->o");

    string = TypeTokenString("o->(o->o)");

    CHECK(string.toString() == "o->(o->o)");

    string = TypeTokenString("[o,o]->o");

    CHECK(string.toString() == "[o,o]->o");

    string = TypeTokenString("[Variable,IndividualConstant,PropositionalType]->PropositionalType");

    CHECK(string.toString() == "[Variable,IndividualConstant,PropositionalType]->PropositionalType");

    //Equality and Inequality
    CHECK(TypeTokenString("[Proposition,Proposition]->Proposition") == TypeTokenString("[Proposition,Proposition]->Proposition"));
    CHECK(TypeTokenString("A->(A->(A->(A->B)))") == TypeTokenString("A->(A->(A->(A->B)))"));

    CHECK(TypeTokenString("A") != TypeTokenString("B"));
}

TEST_CASE("TypeParsingTrees")
{
    TypeParsingTree tree(TypeTokenString("Aflosos"));
    TypeParsingTreeIterator iter(&tree);

    SECTION("TypeParsingTrees Nodes and Iterator")
    {

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

    //Equality and Inequality
    CHECK(TypeParsingTree(TypeTokenString("Aflosis")) == TypeParsingTree(TypeTokenString("Aflosis")));
    CHECK(TypeParsingTree(TypeTokenString("a")) != TypeParsingTree(TypeTokenString("b")));

    //Deep Equality and Copy Constructor
    TypeParsingTree tree2(TypeTokenString("([i,i,i]->i)->o"));
    CHECK(tree2 == tree2);
    TypeParsingTree tree3(tree2);
    CHECK(tree3 == tree2);
    TypeParsingTree tree4(TypeTokenString("a"));
    CHECK(tree3 != tree4);
}

TEST_CASE("Type")
{
    //Validation Pass
    CHECK_NOTHROW(Type("AFLUSISS"));
    CHECK_NOTHROW(Type("Proposition->Proposition"));
    CHECK_NOTHROW(Type("[Proposition,Proposition]->Proposition"));
    CHECK_NOTHROW(Type("[IndividualVariable,IndividualConstant,"
                       "IndividualVariable,IndividualConstant]->Proposition"));
    CHECK_NOTHROW(Type("[IndividualVariable,Proposition]->Proposition"));
    CHECK_NOTHROW(Type("Proposition->(Proposition->Proposition)"));
    CHECK_NOTHROW(Type("(o->o)->o"));
    CHECK_NOTHROW(Type("[o->o,o->o]->o"));
    CHECK_NOTHROW(Type("[a,b,c,d,e,f,g,h,i,j]->k"));
    CHECK_NOTHROW(Type("A->(A->(A->(A->B)))"));

    //Validation Fail
    CHECK_THROWS_WITH(Type(""), "String is empty!");
    CHECK_THROWS_WITH(Type(" "), "' ' is not a suitable expression for a primitive type.");
    CHECK_THROWS_WITH(Type("[[o]->o]->o"), "Product types must have at least two arguments!");
    CHECK_THROWS_WITH(Type("[Proposition]->([Proposition]->Proposition)"), "Product types must have at least two arguments!");
    CHECK_THROWS_WITH(Type("[Proposition]->Proposition"), "Product types must have at least two arguments!");
    CHECK_THROWS_WITH(Type("Proposition->([Proposition]->Proposition)"), "Product types must have at least two arguments!");
    CHECK_THROWS_WITH(Type("[o->o]->o"), "Product types must have at least two arguments!");
    CHECK_THROWS_WITH(Type("(o]->o)"), "Index out of bounds."); //FIXME Need more information!
    CHECK_THROWS_WITH(Type("[,]->o"), "Argument End Offset cannot be less than Argument Begin Offset!"); //FIXME
    CHECK_THROWS_WITH(Type("[o,a)->a"), "Delimiters count do not match!");
    CHECK_THROWS_WITH(Type("o->[i,i]"), "Composition operator's right side argument should be either a composite type or a primitive type!");
    CHECK_THROWS_WITH(Type("o->(o]"), "There are uncased parenthesis in the right side argument of composition operator!");
    CHECK_THROWS_WITH(Type("[0,1]"), "The main type cannot be a product type!");
    CHECK_THROWS_WITH(Type("[[o,o]]->o"), "Product types must have at least two arguments!");
    CHECK_THROWS_WITH(Type("((o->o))->o"), "Index out of bounds."); //FIXME
    CHECK_THROWS_WITH(Type("()->o"), "Type cannot be empty!");
    CHECK_THROWS_WITH(Type("o->(o)"), "Index out of bounds."); //FIXME
    CHECK_THROWS_WITH(Type("o->([a,b,c->d])"), "Index out of bounds."); //FIXME
    CHECK_THROWS_WITH(Type("o->(o,a,ds,r)"), "Composition operator (->) was expected here!");

    //Other
    CHECK(Type("o->o") == Type("o->o"));

    CHECK(Type("o") != Type("a->([c,d]->g)"));

    TypeParsingTree tree = Type("o").getParsingTree();
    Type("i").getParsingTree();

    CHECK_NOTHROW(tree.getHeight());

    //Argument Types, Return Types and Application
    Type t1("i->(i->i)");
    CHECK(t1.getArgumentsTypes() == QVector<Type>({Type("i")}));
    CHECK(t1.getReturnType() == Type("i->i"));
    CHECK(t1.applyArguments(QVector<Type>({Type("i")})) == Type("i->i"));

    Type t2("[i,o,bloggers]->([o,o,o]->i)");
    CHECK(t2.getArgumentsTypes() == QVector<Type>({Type("i"), Type("o"), Type("bloggers")}));
    CHECK(t2.getReturnType() == Type("[o,o,o]->i"));
    CHECK(t2.applyArguments(QVector<Type>({Type("i"), Type("o"), Type("bloggers")})) == Type("[o,o,o]->i"));
}

TEST_CASE("ParsingErrorException")
{
    //Type Parsing Error Exception
    TypeTokenString str("[o,o]->[o,o]");
    ParsingErrorException<TypeTokenString> e("The right hand side argument of the composition operator cannot be a product type!",
                                             6,
                                             10,
                                             str);

    CHECK(QString(e.what().data()) == QString("The right hand side argument of the composition operator cannot be a product type!\n"
                                       "[o,o]->[o,o]\n"
                                       "       ^^^^^"));

}

TEST_CASE("Punctuation Token")
{
    //Validation
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
    SECTION("Validation")
    {
        Type a(Type("o"));

        CHECK_NOTHROW(CoreToken("asdsad", a));
        CHECK_NOTHROW(CoreToken("Abacate", a));
        CHECK_NOTHROW(CoreToken("Afleflesis", a));
        CHECK_NOTHROW(CoreToken("Afleisis", a));
        CHECK_NOTHROW(CoreToken("+", a));
        CHECK_NOTHROW(CoreToken(">", a));
        CHECK_NOTHROW(CoreToken("<", a));
        CHECK_NOTHROW(CoreToken("-", a));
        CHECK_NOTHROW(CoreToken("->", a));
        CHECK_NOTHROW(CoreToken("<-", a));
        CHECK_NOTHROW(CoreToken("&", a));
        CHECK_NOTHROW(CoreToken("=", a));
        CHECK_NOTHROW(CoreToken("|", a));
        CHECK_NOTHROW(CoreToken("\\", a));
        CHECK_NOTHROW(CoreToken("*", a));

        CHECK_THROWS(CoreToken("(", a));
        CHECK_THROWS(CoreToken(")", a));
        CHECK_THROWS(CoreToken(",", a));
        CHECK_THROWS(CoreToken("das sdsd", a));
        CHECK_THROWS(CoreToken("asdsd,dsds", a));
        CHECK_THROWS(CoreToken("IASD(dds", a));
    }
}

TEST_CASE("Lexer, Table Signature and Type Token String")
{
    TableSignature signature;

    SECTION("Token pointers work")
    {
        CHECK(signature.getTokenPointer("(") == signature.getTokenPointer("("));
        CHECK(signature.getTokenPointer(")") == signature.getTokenPointer(")"));

        CHECK_THROWS(signature.getTokenPointer("Aflisis"));
        CHECK_THROWS(signature.getTokenPointer("P"));
        CHECK_THROWS(signature.getTokenPointer("Int"));

        CHECK_THROWS(signature.addToken(PunctuationToken("(")));
        CHECK_THROWS(signature.addToken(PunctuationToken(")")));
    }

    //Some Core Tokens
    signature.addToken(CoreToken("P", Type("i->o")));
    signature.addToken(CoreToken("a", Type("i")));

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
    TableSignature signature;
    signature.addToken(CoreToken("A", Type("i")));

    Lexer lexer(&signature);
    ParsingTree tree(lexer.lex("A"));
    ParsingTreeIterator iter(&tree);

    SECTION("ParsingTrees Nodes methods work")
    {
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


TEST_CASE("Parser Propositional Logic")
{
    //SETUP
    TableSignature signature;

    signature.addToken(CoreToken("P", Type("o")));
    signature.addToken(CoreToken("Q", Type("o")));
    signature.addToken(CoreToken("And", Type("[o,o]->o")));
    signature.addToken(CoreToken("Not", Type("o->o")));
    signature.addToken(CoreToken("Implies", Type("[o,o]->o")));
    signature.addToken(CoreToken("Or", Type("[o,o]->o")));
    signature.addToken(CoreToken("Equivalent", Type("[o,o]->o")));

    Parser parser(&signature, Type("o"));

    //END SETUP

    //Pass

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

    //Fail Due To Structure
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

    //Fail Due To Type Checking
    CHECK_THROWS(parser.parse("(Not (And P Q) P)"));


}

TEST_CASE("Container Auxiliary Tools")
{
    {
        //Check For Duplicates
        QVector<int> v1{1,1,2,3,4};
        QVector<int> v2{3,5,2,5432,6,5432};
        QVector<int> v3{0,1,2,3,4};
        QVector<int> v4{23,1,2,3,4};

        CHECK(ContainerAuxiliaryTools::checkForDuplicates<QVector<int>>(v1));
        CHECK(ContainerAuxiliaryTools::checkForDuplicates<QVector<int>>(v2));
        CHECK(!ContainerAuxiliaryTools::checkForDuplicates<QVector<int>>(v3));
        CHECK(!ContainerAuxiliaryTools::checkForDuplicates<QVector<int>>(v4));
    }

    {
        //Containers Are Disjoint
        QVector<int> v1{1,2,3,4,5};
        QVector<int> v2{5,6,7,8};
        QVector<int> v3{2,3,4};

        CHECK(!ContainerAuxiliaryTools::containersAreDisjoint<QVector<int>>(v1,v2));
        CHECK(!ContainerAuxiliaryTools::containersAreDisjoint<QVector<int>>(v1,v3));
        CHECK(ContainerAuxiliaryTools::containersAreDisjoint<QVector<int>>(v2,v3));
    }

}

TEST_CASE("Binding and Variable Tokens")
{
    //Variable Token
    VariableToken v1("Aflisis", Type("Prop->Prop"));
    CHECK(v1.tokenClass() == "VariableToken");

    //Binding Token
    QVector<BindingRecord> records;

    BindingRecord record1(0, QVector<unsigned int>{1});
    records.push_back(record1);

    CHECK_NOTHROW(BindingToken("Forall", Type("[i,o]->o"), records));

    CHECK_THROWS(BindingToken("P", Type("i->o"), records));
    CHECK_THROWS(BindingToken("Q", Type("o"), records));
}

TEST_CASE("First Order Logic (With TableSignature)")
{
    TableSignature signature;

    signature.addToken(CoreToken("And", Type("[o,o]->o")));
    signature.addToken(CoreToken("Or", Type("[o,o]->o")));
    signature.addToken(CoreToken("Implies", Type("[o,o]->o")));
    signature.addToken(CoreToken("Equivalent", Type("[o,o]->o")));
    signature.addToken(CoreToken("Not", Type("o->o")));
    signature.addToken(CoreToken("P", Type("i->o")));
    signature.addToken(CoreToken("G", Type("i->o")));
    signature.addToken(CoreToken("R", Type("[i,i]->o")));
    signature.addToken(CoreToken("a", Type("i")));
    signature.addToken(CoreToken("b", Type("i")));

    signature.addToken(VariableToken("x", Type("i")));
    signature.addToken(VariableToken("y", Type("i")));

    QVector<BindingRecord> records;
    records.push_back(BindingRecord(0, QVector<unsigned int>{1}));

    signature.addToken(BindingToken("Forall", Type("[i,o]->o"), records));
    signature.addToken(BindingToken("Exists", Type("[i,o]->o"), records));

    signature.addToken(VariableToken("z", Type("i")));
    signature.addToken(CoreToken("Equals", Type("[i,i]->o")));

    Parser parser(&signature, Type("o"));

    //Pass
    CHECK_NOTHROW(parser.parse("(Forall x (Implies (P x) (P x)))"));
    CHECK_NOTHROW(parser.parse("(Forall x (Exists y (R x y)))"));
    CHECK_NOTHROW(parser.parse("(Forall x (Implies (P x) (Exists y (And (R x y) (Forall z (Implies (R x z) (Equals z y)))   ) ) ) )"));

    //Fail
    CHECK_THROWS(parser.parse("(Forall y (Implies (P x) (P x)))"));
    CHECK_THROWS(parser.parse("(Forall x (Forall x (P x)))"));
}

TEST_CASE("Elementary Arithmetic (Table Signature)")
{
    TableSignature signature;

    signature.addToken(CoreToken("0", Type("i")));
    signature.addToken(CoreToken("1", Type("i")));
    signature.addToken(CoreToken("2", Type("i")));
    signature.addToken(CoreToken("3", Type("i")));
    signature.addToken(CoreToken("4", Type("i")));
    signature.addToken(CoreToken("5", Type("i")));
    signature.addToken(CoreToken("6", Type("i")));
    signature.addToken(CoreToken("7", Type("i")));
    signature.addToken(CoreToken("8", Type("i")));
    signature.addToken(CoreToken("9", Type("i")));
    signature.addToken(CoreToken("Plus", Type("[i,i]->i")));
    signature.addToken(CoreToken("Minus", Type("[i,i]->i")));
    signature.addToken(CoreToken("Times", Type("[i,i]->i")));
    signature.addToken(CoreToken("Div", Type("[i,i]->i")));

    Parser parser(&signature, Type("i"));

    //Pass
    CHECK_NOTHROW(parser.parse("1"));
    CHECK_NOTHROW(parser.parse("7"));
    CHECK_NOTHROW(parser.parse("9"));

    CHECK_NOTHROW(parser.parse("(Plus 1 3)"));
    CHECK_NOTHROW(parser.parse("(Times (Plus 2 3) (Plus 4 7))"));
    CHECK_NOTHROW(parser.parse("(Div (Times (Plus 3 4) 5) (Plus (Times 0 2) (Minus 3 5)))"));

    //Fail

}

TEST_CASE("BasicProcessorTokenRecord")
{
    CHECK_THROWS(BasicProcessorTokenRecord(CoreToken("A", Type("i")), 0, BasicProcessorTokenRecord::Associativity::Left));
    CHECK_THROWS(BasicProcessorTokenRecord(CoreToken("A", Type("i->o")), 2, BasicProcessorTokenRecord::Associativity::Left));
    CHECK_THROWS(BasicProcessorTokenRecord(CoreToken("A", Type("[i,i,i]")), 4, BasicProcessorTokenRecord::Associativity::Left));

    CHECK_NOTHROW(BasicProcessorTokenRecord(CoreToken("A", Type("i->(i->o)")), 1, BasicProcessorTokenRecord::Associativity::Right));
}

TEST_CASE("BasicPreProcessor")
{
    TableSignature signature;

    signature.addToken(CoreToken("Operator", Type("i->o")));
    signature.addToken(CoreToken("YubbaDubba", Type("[i,i,i]->o")));
    signature.addToken(CoreToken("a", Type("i")));
    signature.addToken(CoreToken("YabbaDabba", Type("[i,i,i]->o")));

    BasicPreProcessor processor(&signature);

    CHECK_NOTHROW(processor.addTokenRecord("Operator", 1, 0, BasicProcessorTokenRecord::Associativity::Left));
    CHECK_NOTHROW(processor.addTokenRecord("YubbaDubba", 3, 2, BasicProcessorTokenRecord::Associativity::Left));

    CHECK_THROWS(processor.addTokenRecord("Operator", 1, 0, BasicProcessorTokenRecord::Associativity::Left));
    CHECK_THROWS(processor.addTokenRecord("a", 0, 1, BasicProcessorTokenRecord::Associativity::Left));
    CHECK_THROWS(processor.addTokenRecord("YabbaDabba", 4, 2, BasicProcessorTokenRecord::Associativity::Left));

    //processor.removeTokenRecord("Operator"); FIXME! Gotta fix this!
    //FIXME! asd

    //CHECK_NOTHROW(processor.addTokenRecord("Operator", 1, 0, BasicProcessorTokenRecord::Associativity::Left));
}

TEST_CASE("Token String Methods")
{
    TableSignature signature;

    signature.addToken(CoreToken("0", Type("i")));
    signature.addToken(CoreToken("1", Type("i")));
    signature.addToken(CoreToken("2", Type("i")));
    signature.addToken(CoreToken("3", Type("i")));
    signature.addToken(CoreToken("4", Type("i")));
    signature.addToken(CoreToken("5", Type("i")));
    signature.addToken(CoreToken("6", Type("i")));
    signature.addToken(CoreToken("7", Type("i")));
    signature.addToken(CoreToken("8", Type("i")));
    signature.addToken(CoreToken("9", Type("i")));

    Lexer lexer(&signature);

    TokenString tokenString = lexer.lex("0 1 2 3 4 6 7 8 9");
    //tokenString.insert(5, "5");
    //CHECK(tokenString.toString() == "0123456789");
}

TEST_CASE("PreProcessed Elementary One Digit Binary Arithmetic")
{
    TableSignature signature;

    signature.addToken(CoreToken("0", Type("i")));
    signature.addToken(CoreToken("1", Type("i")));
    signature.addToken(CoreToken("Plus", Type("[i,i]->i")));
    signature.addToken(CoreToken("Minus", Type("i->i")));
    signature.addToken(CoreToken("Times", Type("[i,i]->i")));

    Parser parser(&signature, Type("i"));

    SECTION("Left Associativity")
    {
        BasicPreProcessor preProcessor(&signature);

        preProcessor.addTokenRecord("Minus", 0, 0, BasicProcessorTokenRecord::Associativity::Left);
        preProcessor.addTokenRecord("Times", 1, 1, BasicProcessorTokenRecord::Associativity::Left);
        preProcessor.addTokenRecord("Plus", 1, 2, BasicProcessorTokenRecord::Associativity::Left);

        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times 0 Plus 1 Plus Minus 0")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times 0 Plus 1 Plus (Minus 0)")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times 0 Plus (1 Plus (Minus 0))")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times (0 Plus (1 Plus (Minus 0)))")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("(1 Times (0 Plus (1 Plus (Minus 0))))")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("(1 Times 0) Plus 1 Plus Minus 0")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("((1 Times 0) Plus 1) Plus Minus 0")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Plus 0 Plus 1 Plus 1 Times 0")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Plus Minus 0 Times 1")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Plus (Minus 0) Times 1")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("(1 Plus Minus 1) Times 1")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("Minus 1 Plus (Minus 0) Times 1")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("(Minus 1 Plus Minus 1) Times 0")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times 1 Times 0")));
        CHECK_NOTHROW(parser.parse(preProcessor.processString("(1 Times 1) Times 0")));

        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times (0 Times 1)")));



    }


    SECTION("Right Associativity")
    {
        BasicPreProcessor preProcessor(&signature);

        preProcessor.addTokenRecord("Minus", 0, 0, BasicProcessorTokenRecord::Associativity::Right);
        preProcessor.addTokenRecord("Times", 1, 1, BasicProcessorTokenRecord::Associativity::Right);
        preProcessor.addTokenRecord("Plus", 1, 2, BasicProcessorTokenRecord::Associativity::Right);
        CHECK_NOTHROW(parser.parse(preProcessor.processString("1 Times 0 Plus 1 Plus Minus 0")));
    }



}

TEST_CASE("Pre and Post Processed One Digit Arithmetic")
{
    TableSignature signature;

    signature.addToken(CoreToken("0", Type("i")));
    signature.addToken(CoreToken("1", Type("i")));
    signature.addToken(CoreToken("2", Type("i")));
    signature.addToken(CoreToken("3", Type("i")));
    signature.addToken(CoreToken("4", Type("i")));
    signature.addToken(CoreToken("5", Type("i")));
    signature.addToken(CoreToken("6", Type("i")));
    signature.addToken(CoreToken("7", Type("i")));
    signature.addToken(CoreToken("8", Type("i")));
    signature.addToken(CoreToken("9", Type("i")));
    signature.addToken(CoreToken("Minus", Type("i->i")));
    signature.addToken(CoreToken("Plus", Type("[i,i]->i")));
    signature.addToken(CoreToken("Times", Type("[i,i]->i")));
    signature.addToken(CoreToken("Power", Type("[i,i]->i")));
    signature.addToken(CoreToken("CPlus", Type("i->(i->i)")));

    Parser parser(&signature, Type("i"));

    //PreProcessor Setup
    BasicPreProcessor preProcessor(&signature);

    preProcessor.addTokenRecord("Plus", 1);
    preProcessor.addTokenRecord("Times", 1);
    preProcessor.addTokenRecord("Power", 1);
    preProcessor.addTokenRecord("Minus", 0);

    //PostProcessor Setup
    BasicPostProcessor postProcessor(&signature);

    postProcessor.addTokenRecord("Plus", 1);
    postProcessor.addTokenRecord("Times", 1);
    postProcessor.addTokenRecord("Power", 1);
    postProcessor.addTokenRecord("Minus", 0);

    //Tests
    CHECK(postProcessor.processString(parser.parse(preProcessor.processString("1 Times 0 Plus 1 Plus Minus 0")).formattedString()) == "1 Times 0 Plus 1 Plus Minus 0");
    CHECK(postProcessor.processString(parser.parse(preProcessor.processString("Minus (2 Power (3 Plus 4) Times 5)")).formattedString()).toStdString() == "Minus (2 Power (3 Plus 4) Times 5)");

    //Curried Plus
    CHECK_NOTHROW(parser.parse(preProcessor.processString("((CPlus 2) 3 Plus 5)")));

}

TEST_CASE("Formatter")
{
    //Enviroment Setup
    TableSignature signature;
    signature.addToken(CoreToken("P", Type("o")));
    signature.addToken(CoreToken("Q", Type("o")));
    signature.addToken(CoreToken("^", Type("[o,o]->o")));
    signature.addToken(CoreToken("->", Type("[o,o]->o")));
    signature.addToken(CoreToken("~", Type("o->o")));

    BasicPreProcessor preProcessor(&signature);
    preProcessor.addTokenRecord("->", 1);
    preProcessor.addTokenRecord("^", 1);
    preProcessor.addTokenRecord("~", 0);

    BasicPostProcessor postProcessor(&signature);
    postProcessor.addTokenRecord("->", 1);
    postProcessor.addTokenRecord("^", 1);
    postProcessor.addTokenRecord("~", 0);

    //Testing -> We need more processors to test this thoroughly or at least create dummy processors
    Formatter preFormatter, postFormatter;
    preFormatter.addProcessor(&preProcessor);
    postFormatter.addProcessor(&postProcessor);
    CHECK(preFormatter.format("P ^ ~ Q -> ~ Q ^ P") == "(-> (^ P (~ Q)) (^ (~ Q) P))");
    CHECK(postFormatter.format("(-> (^ P (~ Q)) (^ (~ Q) P))").toStdString() == "P ^ ~ Q -> ~ Q ^ P");

    //Serialization
    QBuffer buffer;
    QDataStream stream(&buffer);
    buffer.open(QIODevice::WriteOnly);
    stream << preFormatter << postFormatter;

    buffer.close();
    buffer.open(QIODevice::ReadOnly);
    Formatter preFormatter2(stream, QVector<StringProcessor *>({&preProcessor})),
              postFormatter2(stream, QVector<StringProcessor *>({&postProcessor}));

    CHECK(preFormatter2.format("P ^ ~ Q -> ~ Q ^ P") == "(-> (^ P (~ Q)) (^ (~ Q) P))");
    CHECK(postFormatter2.format("(-> (^ P (~ Q)) (^ (~ Q) P))").toStdString() == "P ^ ~ Q -> ~ Q ^ P");

    buffer.close();
    buffer.open(QIODevice::ReadOnly);
    Formatter preFormatter3, postFormatter3;
    preFormatter3.unserialize(stream, QVector<StringProcessor *>({&preProcessor}));
    postFormatter3.unserialize(stream, QVector<StringProcessor *>({&postProcessor}));

    CHECK(preFormatter3.format("P ^ ~ Q -> ~ Q ^ P") == "(-> (^ P (~ Q)) (^ (~ Q) P))");
    CHECK(postFormatter3.format("(-> (^ P (~ Q)) (^ (~ Q) P))").toStdString() == "P ^ ~ Q -> ~ Q ^ P");

}

TEST_CASE("First Order Logic With Pre and Post Processor")
{
    //Language
    TableSignature signature;

    signature.addToken(CoreToken("~", Type("o->o")));
    signature.addToken(CoreToken("&", Type("[o,o]->o")));
    signature.addToken(CoreToken("|", Type("[o,o]->o")));
    signature.addToken(CoreToken("->", Type("[o,o]->o")));
    signature.addToken(CoreToken("<->", Type("[o,o]->o")));

    signature.addToken(CoreToken("P", Type("i->o")));
    signature.addToken(CoreToken("Q", Type("i->o")));
    signature.addToken(CoreToken("R", Type("[i,i]->o")));

    signature.addToken(CoreToken("a", Type("i")));
    signature.addToken(CoreToken("b", Type("i")));
    signature.addToken(CoreToken("c", Type("i")));

    signature.addToken(VariableToken("x", Type("i")));
    signature.addToken(VariableToken("y", Type("i")));
    signature.addToken(VariableToken("z", Type("i")));


    BindingRecord quantifierRecord(0, QVector<unsigned int>{1});

    QVector<BindingRecord> bindingRecords;
    bindingRecords.push_back(quantifierRecord);

    signature.addToken(BindingToken("U", Type("[i,o]->o"), bindingRecords));
    signature.addToken(BindingToken("E", Type("[i,o]->o"), bindingRecords));

    //Pre Processor
    BasicPreProcessor preProcessor(&signature);

    preProcessor.addTokenRecord("~", 0, 0, BasicProcessorTokenRecord::Associativity::Left);
    preProcessor.addTokenRecord("&", 1, 1, BasicProcessorTokenRecord::Associativity::Left);
    preProcessor.addTokenRecord("|", 1, 2, BasicProcessorTokenRecord::Associativity::Left);
    preProcessor.addTokenRecord("->", 1, 3, BasicProcessorTokenRecord::Associativity::Right);
    preProcessor.addTokenRecord("<->", 1, 4, BasicProcessorTokenRecord::Associativity::Left);
    preProcessor.addTokenRecord("U", 0, 5, BasicProcessorTokenRecord::Associativity::Right);
    preProcessor.addTokenRecord("E", 0, 5, BasicProcessorTokenRecord::Associativity::Right);

    preProcessor.insertTokenRecord("P", 0, 0, BasicProcessorTokenRecord::Associativity::Right);
    preProcessor.addTokenRecord("Q", 0, 0, BasicProcessorTokenRecord::Associativity::Right);
    preProcessor.addTokenRecord("R", 0, 0, BasicProcessorTokenRecord::Associativity::Right);

    //Post Processor
    BasicPostProcessor postProcessor(&signature);

    postProcessor.addTokenRecord("~", 0, 0, BasicProcessorTokenRecord::Associativity::Left);
    postProcessor.addTokenRecord("&", 1, 1, BasicProcessorTokenRecord::Associativity::Left);
    postProcessor.addTokenRecord("|", 1, 2, BasicProcessorTokenRecord::Associativity::Left);
    postProcessor.addTokenRecord("->", 1, 3, BasicProcessorTokenRecord::Associativity::Right);
    postProcessor.addTokenRecord("<->", 1, 4, BasicProcessorTokenRecord::Associativity::Left);
    postProcessor.addTokenRecord("U", 0, 5, BasicProcessorTokenRecord::Associativity::Right);
    postProcessor.addTokenRecord("E", 0, 5, BasicProcessorTokenRecord::Associativity::Right);

    postProcessor.insertTokenRecord("P", 0, 0, BasicProcessorTokenRecord::Associativity::Right);
    postProcessor.addTokenRecord("Q", 0, 0, BasicProcessorTokenRecord::Associativity::Right);
    postProcessor.addTokenRecord("R", 0, 0, BasicProcessorTokenRecord::Associativity::Right);
    //Parser
    Parser parser(&signature, Type("o"));

    SECTION("Pre Processing Only")
    {
        CHECK(parser.parse(preProcessor.processString("P a")).formattedString() == "(P a)");
        CHECK(parser.parse(preProcessor.processString("P a & Q b")).formattedString() == "(& (P a) (Q b))");
        CHECK(parser.parse(preProcessor.processString("P a & Q b & R a c")).formattedString() == "(& (& (P a) (Q b)) (R a c))");
    //    CHECK(parser.parse(preProcessor.processString("P a | Q b & R  a c")).formattedString() == "P a | Q b & R a c");
    //    CHECK(parser.parse(preProcessor.processString("(P a | Q b) & R  a c")).formattedString() == "(P a | Q b) & R a c");
    //    CHECK(parser.parse(preProcessor.processString("(P a | Q b) & R  a c")).formattedString() == "(P a | Q b) & R a c");
    //    CHECK(parser.parse(preProcessor.processString("P a & P b -> P c")).formattedString() == "P a & P b -> P c");
    }

    SECTION("Pre and Post Processing")
    {
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("P a")).formattedString()) == "P a");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("P a & Q b")).formattedString()) == "P a & Q b");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("P a & Q b & R  a c")).formattedString()).toStdString() == "P a & Q b & R a c");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("P a | Q b & R  a c")).formattedString()) == "P a | Q b & R a c");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("(P a | Q b) & R a c")).formattedString()) == "(P a | Q b) & R a c");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("(P a | Q b) & R a c")).formattedString()) == "(P a | Q b) & R a c");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("P a & P b -> P c")).formattedString()) == "P a & P b -> P c");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x P x -> Q x")).formattedString()) == "U x P x -> Q x");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x P x -> R x x")).formattedString()) == "U x P x -> R x x");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U y U x R x y -> R y x")).formattedString()).toStdString() == "U y U x R x y -> R y x");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("E y U x R x y -> R y x")).formattedString()).toStdString() == "E y U x R x y -> R y x");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("(U x U y R x y -> ~ R y x) -> (U x ~ R x x) ")).formattedString()).toStdString() == "(U x U y R x y -> ~ R y x) -> (U x ~ R x x)");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x E y R x y")).formattedString()).toStdString() == "U x E y R x y");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("E x U y R x y")).formattedString()).toStdString() == "E x U y R x y");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x E y U z R x y <-> P z")).formattedString()).toStdString() == "U x E y U z R x y <-> P z");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x U y U z R x y & R y z -> R x z")).formattedString()).toStdString() == "U x U y U z R x y & R y z -> R x z");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("(U x P x -> Q x) & (E x P x) -> (E x Q x)")).formattedString()).toStdString() == "(U x P x -> Q x) & (E x P x) -> (E x Q x)");

    }

    SECTION("Failures")
    {
        SECTION("Structural Errors")
        {
            CHECK_THROWS(parser.parse("("));
            CHECK_THROWS(parser.parse(")"));
            CHECK_THROWS(parser.parse("()"));
            CHECK_THROWS(parser.parse("(a)"));
            CHECK_THROWS(parser.parse(")P a("));
            CHECK_THROWS(parser.parse("P a"));
            CHECK_THROWS(parser.parse("(P a"));
            CHECK_THROWS(parser.parse("P a)"));
        }

        SECTION("Type Errors")
        {
            CHECK_THROWS(parser.parse("(P Q)"));
            CHECK_THROWS(parser.parse("(b P)"));
            CHECK_THROWS(parser.parse("(R a)"));
        }

        SECTION("Variable Binding Errors")
        {}
    }

    signature.addToken(CoreToken("f", Type("i->i")));
    signature.addToken(CoreToken("g", Type("i->i")));

    preProcessor.insertTokenRecord("f", 0, 0);
    preProcessor.addTokenRecord("g", 0, 0);

    postProcessor.insertTokenRecord("f", 0, 0);
    postProcessor.addTokenRecord("g", 0, 0);

    SECTION("Functional Symbols")
    {
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("P f a")).formattedString()) == "P f a");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x P f x")).formattedString()) == "U x P f x");
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("U x U y R x y <-> R x f x")).formattedString()) == "U x U y R x y <-> R x f x");
    }

    QVector<BindingRecord> lambdaRecord;
    lambdaRecord.push_back(BindingRecord(0, QVector<unsigned int>{1}));
    signature.addToken(BindingToken("Lambda", Type("[i,i]->i"), lambdaRecord));

    preProcessor.insertTokenRecord("Lambda", 0, 1);

    postProcessor.insertTokenRecord("Lambda", 0, 1);

    SECTION("Lambda Expressions")
    {
        CHECK(postProcessor.processString(parser.parse(preProcessor.processString("R Lambda x f x Lambda y g y")).formattedString()) == "R Lambda x f x Lambda y g y");
    }

}

TEST_CASE("Serialization")
{
    QFile file("C:/Users/Henrique/Documents/Qt Projects/Language/test.dat");

    SECTION("Type Token")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        TypeToken t1("i"), t2("o"), t3("->"), t4("["), t5("]"), t6("("), t7(")");
        stream << t1 << t2 << t3 << t4 << t5 << t6 << t7;

        file.reset();

        TypeToken t8(stream), t9(stream), t10(stream), t11(stream), t12(stream), t13(stream), t14(stream);

        CHECK(t1 == t8);
        CHECK(t2 == t9);
        CHECK(t3 == t10);
        CHECK(t4 == t11);
        CHECK(t5 == t12);
        CHECK(t6 == t13);
        CHECK(t7 == t14);
    }

    SECTION("Type Token String")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        TypeTokenString t1("i"), t2("i->o"), t3("[i,i,i]->o");
        stream << t1 << t2 << t3;

        file.reset();

        TypeTokenString t4(stream), t5(stream), t6(stream);

        CHECK(t1 == t4);
        CHECK(t2 == t5);
        CHECK(t3 == t6);
    }

    SECTION("Type")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        Type t1("i"), t2("i->o"), t3("(i->o)->o"), t4("[o,o]->o");
        stream << t1 << t2 << t3 << t4;

        file.reset();

        Type t5(stream), t6(stream), t7(stream), t8(stream);

        CHECK(t1 == t5);
        CHECK(t2 == t6);
        CHECK(t3 == t7);
        CHECK(t4 == t8);
    }

    SECTION("Core Token")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        CoreToken t1("P", Type("i")), t2("And", Type("o")), t3("Or", Type("i->o"));
        stream << t1 << t2 << t3;

        file.reset();

        unique_ptr<Token> t4(Token::unserializePtr(stream));
        unique_ptr<Token> t5(Token::unserializePtr(stream));
        unique_ptr<Token> t6(Token::unserializePtr(stream));

        CHECK(t1 == *t4);
        CHECK(t2 == *t5);
        CHECK(t3 == *t6);
    }

    SECTION("Punctuaction Token")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        PunctuationToken t1("("), t2(")");
        stream << t1 << t2;

        file.reset();

        unique_ptr<Token> t3(Token::unserializePtr(stream));
        unique_ptr<Token> t4(Token::unserializePtr(stream));

        CHECK(t1 == *t3);
        CHECK(t2 == *t4);
    }

    SECTION("Variable Token")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        VariableToken t1("x", Type("i")), t2("y", Type("o")), t3("z", Type("p"));
        stream << t1 << t2 << t3;

        file.reset();

        unique_ptr<Token> t4(Token::unserializePtr(stream));
        unique_ptr<Token> t5(Token::unserializePtr(stream));
        unique_ptr<Token> t6(Token::unserializePtr(stream));

        CHECK(t1 == *t4);
        CHECK(t2 == *t5);
        CHECK(t3 == *t6);
    }

    SECTION("Binding Token")
    {
        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        BindingRecord record(0, QVector<unsigned int>{1});
        BindingToken t1("Forall", Type("[i,o]->o"), QVector<BindingRecord>{record});
        stream << t1;

        file.reset();

        unique_ptr<Token> t2(Token::unserializePtr(stream));

        CHECK(t1 == *t2);
    }

    SECTION("Token String")
    {
        TableSignature signature;

        signature.addToken(CoreToken("~", Type("o->o")));
        signature.addToken(CoreToken("&", Type("[o,o]->o")));
        signature.addToken(CoreToken("|", Type("[o,o]->o")));
        signature.addToken(CoreToken("->", Type("[o,o]->o")));
        signature.addToken(CoreToken("<->", Type("[o,o]->o")));

        signature.addToken(CoreToken("P", Type("i->o")));
        signature.addToken(CoreToken("Q", Type("i->o")));
        signature.addToken(CoreToken("R", Type("[i,i]->o")));

        signature.addToken(CoreToken("a", Type("i")));
        signature.addToken(CoreToken("b", Type("i")));
        signature.addToken(CoreToken("c", Type("i")));

        Lexer lexer(&signature);

        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        TokenString t1, t2, t3;
        t1 = lexer.lex("(-> (~ (P a)) (~ (R b c)))");
        t2 = lexer.lex("(| (P c) (& (P a) (P b)))");
        t3 = lexer.lex("(Q a)");

        stream << t1 << t2 << t3;

        file.reset();

        TokenString t4(stream, &signature), t5(stream, &signature), t6(stream, &signature);

        CHECK(t1 == t4);
        CHECK(t2 == t5);
        CHECK(t3 == t6);
    }

    SECTION("Table Signature")
    {
        TableSignature signature;

        signature.addToken(CoreToken("~", Type("o->o")));
        signature.addToken(CoreToken("&", Type("[o,o]->o")));
        signature.addToken(CoreToken("|", Type("[o,o]->o")));
        signature.addToken(CoreToken("->", Type("[o,o]->o")));
        signature.addToken(CoreToken("<->", Type("[o,o]->o")));

        signature.addToken(CoreToken("P", Type("i->o")));
        signature.addToken(CoreToken("Q", Type("i->o")));
        signature.addToken(CoreToken("R", Type("[i,i]->o")));

        signature.addToken(CoreToken("a", Type("i")));
        signature.addToken(CoreToken("b", Type("i")));
        signature.addToken(CoreToken("c", Type("i")));

        file.open(QIODevice::ReadWrite);
        QDataStream stream(&file);

        stream << signature;

        file.reset();

        TableSignature signature2;

        stream >> signature2;

        CHECK(signature.equalTokenTable(signature2));
    }

    SECTION("Formulas")
    {
        TableSignature signature;

        signature.addToken(CoreToken("P", Type("o")));
        signature.addToken(CoreToken("&", Type("[o,o]->o")));

        Parser parser(&signature, Type("o"));

        Formula f1 = parser.parse("P");
        Formula f2 = parser.parse("(& P P)");
        Formula f3 = parser.parse("(& (& P P ) P)");
        Formula f4 = parser.parse("(& P (& P P))");

        file.open(QIODevice::ReadWrite);
        QDataStream stream (&file);

        stream << f1 << f2 << f3 << f4;

        file.reset();

        Formula f5(stream, &signature), f6(stream, &signature), f7(stream, &signature), f8(stream, &signature);


        CHECK(f1 == f5);
        CHECK(f2 == f6);
        CHECK(f3 == f7);
        CHECK(f4 == f8);
    }

}
