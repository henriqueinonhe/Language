#include <QCoreApplication>
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#ifndef CATCH_CONFIG_MAIN

#include <memory>

class A
{
public:
    static std::shared_ptr<int> a;
};

int main()
{

    return 0;
}

#endif
