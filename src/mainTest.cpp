//
//  mainTest.cpp
//  GoogleTest
//
//  Created by 澤田 泰治 on 2012/11/28.
//  Copyright (c) 2012年 澤田 泰治. All rights reserved.
//

#include "mainTest.hpp"
#include <gtest/gtest.h>
using namespace std;

int main( int argc, char* argv[] ) {
    testing::InitGoogleTest( &argc, argv );
    int result = RUN_ALL_TESTS();
    return result;
}
