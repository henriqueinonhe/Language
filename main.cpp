#include <QCoreApplication>
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#ifndef CATCH_CONFIG_MAIN

#include <memory>
#include "typeparser.h"
#include "type.h"

int main()
{
    Type("o");
    Type("a");
    Type("i");

    std::cout << "Aflisis!";
    return 0;
}

#endif
