QT += core
QT -= gui

CONFIG += c++11

TARGET = Language
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    tests.cpp \
    tree.cpp \
    treenode.cpp \
    treeiterator.cpp \
    token.cpp \
    typeparsingtree.cpp \
    typeparsingtreenode.cpp \
    typeparsingtreeiterator.cpp \
    type.cpp \
    typetoken.cpp \
    typetokenstring.cpp \
    tokenstring.cpp \
    lexer.cpp \
    signature.cpp \
    punctuationtoken.cpp \
    coretoken.cpp \
    tablesignature.cpp \
    parsingtree.cpp \
    parsingtreenode.cpp \
    parsingtreeiterator.cpp \
    typeparser.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    catch.hpp \
    token.h \
    typeparsingtree.h \
    typeparsingtreenode.h \
    typeparsingtreeiterator.h \
    type.h \
    typetoken.h \
    pool.h \
    typetokenstring.h \
    parsingauxiliarytools.h \
    tokenstring.h \
    parsingerrorexception.hpp \
    lexer.h \
    signature.h \
    punctuationtoken.h \
    coretoken.h \
    tablesignature.h \
    parsingtree.h \
    parsingtreenode.h \
    parsingtreeiterator.h \
    typeparser.h
