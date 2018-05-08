#include "catch.hpp"
#include "tree.h"
#include "treenode.h"
#include "treeiterator.h"
#include "typeparsingtree.h"
#include "typeparsingtreeiterator.h"
#include "typeparsingtreenode.h"
#include "type.h"
#include "token.h"
#include "pool.h"
#include "typetokenstring.h"
#include <QRegularExpression>
#include <iostream>


TEST_CASE("Trees")
{
    SECTION("Trees Nodes methods work")
    {
        Tree tree;
        TreeIterator iter(&tree);

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->getHeight() == 0);
        CHECK(iter->isRoot());

        iter->appendChild();
        iter.goToChild(0);

        CHECK(iter->coordinatesToString() == QString("(0)"));
        CHECK(iter->getHeight() == 1);

        iter.goToParent();

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->getHeight() == 0);

        iter->appendChild();
        iter.goToChild(1);

        CHECK(iter->coordinatesToString() == QString("(1)"));
        CHECK(iter->getHeight() == 1);

        iter->appendChild();
        iter.goToChild(0);

        CHECK(iter->coordinatesToString() == QString("(1,0)"));
        CHECK(iter->getHeight() == 2);

        iter.goToRoot();

        CHECK(iter->coordinatesToString() == QString("()"));
        CHECK(iter->isRoot());
    }

    SECTION("Tree methods and Tree Height")
    {
        Tree tree;
        TreeIterator iter(&tree);

        CHECK(iter.getTree().getHeight() == 0);

        iter->appendChild();

        CHECK(iter.getTree().getHeight() == 1);

        iter.goToChild(0);

        iter->appendChild();

        CHECK(iter.getTree().getHeight() == 2);

        iter.goToParent();

        iter->appendChild();

        CHECK(iter.getTree().getHeight() == 2);
    }

    SECTION("Tree Iterator Paths")
    {
        Tree tree;
        TreeIterator iter(&tree);

        iter->appendChild();
        iter->appendChild();

        iter.goToChild(1);

        iter->appendChild();
        iter->appendChild();

        iter.goToChild(0);

        iter->appendChild();
        iter->appendChild();

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

}

TEST_CASE("TypeParsingTree Printer")
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
    iter->setMainOperator(TypeParsingTreeNode::MainOperator::Union);

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
        CHECK_NOTHROW(Type("AFLUSISS"));
        CHECK_NOTHROW(Type("[Proposition]->Proposition"));
        CHECK_NOTHROW(Type("[Proposition,Proposition]->Proposition"));
        CHECK_NOTHROW(Type("[IndividualVariable,IndividualConstant,"
                           "IndividualVariable,IndividualConstant]->Proposition"));
        CHECK_NOTHROW(Type("[IndividualVariable,Proposition]->Proposition"));
        CHECK_NOTHROW(Type("[Proposition]->([Proposition]->Proposition)"));
        CHECK_NOTHROW(Type("[[o]->o]->o"));
        CHECK_NOTHROW(Type("[[o]->o,[o]->o]->o"));
        CHECK_NOTHROW(Type("[a,b,c,d,e,f,g,h,i,j]->k"));
    }

    SECTION("Fail")
    {
        CHECK_THROWS(Type(""));
    }
}
