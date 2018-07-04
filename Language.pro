QT += core
QT -= gui

CONFIG += c++11

TARGET = Language
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += Parser \
               Processors \
               Processors/PreProcessors \
               Processors/PostProcessors \
               Token \
               Type \
               Utils

SOURCES += main.cpp \
    tests.cpp \
    Parser/formula.cpp \
    Parser/lexer.cpp \
    Parser/parser.cpp \
    Parser/parsingtree.cpp \
    Parser/parsingtreeiterator.cpp \
    Parser/parsingtreenode.cpp \
    Parser/signature.cpp \
    Parser/tablesignature.cpp \
    Processors/PostProcessors/basicpostprocessor.cpp \
    Processors/PreProcessors/basicpreprocessor.cpp \
    Processors/PreProcessors/classicfunctionnotationpreprocessor.cpp \
    Processors/basicprocessor.cpp \
    Processors/basicprocessortokenrecord.cpp \
    Processors/formatter.cpp \
    Processors/stringprocessor.cpp \
    Token/bindingrecord.cpp \
    Token/bindingtoken.cpp \
    Token/coretoken.cpp \
    Token/punctuationtoken.cpp \
    Token/token.cpp \
    Token/tokenstring.cpp \
    Token/variabletoken.cpp \
    Type/type.cpp \
    Type/typeparser.cpp \
    Type/typeparsingtree.cpp \
    Type/typeparsingtreeiterator.cpp \
    Type/typeparsingtreenode.cpp \
    Type/typetoken.cpp \
    Type/typetokenstring.cpp \
    Utils/dirtyfix.cpp \
    main.cpp \
    tests.cpp

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
    Parser/formula.h \
    Parser/lexer.h \
    Parser/parser.h \
    Parser/parsingerrorexception.hpp \
    Parser/parsingtree.h \
    Parser/parsingtreeiterator.h \
    Parser/parsingtreenode.h \
    Parser/signature.h \
    Parser/tablesignature.h \
    Processors/PostProcessors/basicpostprocessor.h \
    Processors/PreProcessors/basicpreprocessor.h \
    Processors/PreProcessors/classicfunctionnotationpreprocessor.h \
    Processors/basicprocessor.h \
    Processors/basicprocessortokenrecord.h \
    Processors/formatter.h \
    Processors/stringprocessor.h \
    Token/bindingrecord.h \
    Token/bindingtoken.h \
    Token/coretoken.h \
    Token/punctuationtoken.h \
    Token/token.h \
    Token/tokenstring.h \
    Token/variabletoken.h \
    Type/type.h \
    Type/typeparser.h \
    Type/typeparsingerrorexception.h \
    Type/typeparsingtree.h \
    Type/typeparsingtreeiterator.h \
    Type/typeparsingtreenode.h \
    Type/typetoken.h \
    Type/typetokenstring.h \
    Utils/containerauxiliarytools.h \
    Utils/dirtyfix.h \
    Utils/parsingauxiliarytools.h \
    Utils/pool.h \
    catch.hpp
