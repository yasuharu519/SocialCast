//  
//  Created by 澤田 泰治 on 2012/12/03.
//  Copyright (c) 2012年 澤田 泰治. All rights reserved.
//

#ifndef __UTILITY_FUNCTIONS_HPP__
#define __UTILITY_FUNCTIONS_HPP__

#include <iostream>
#include <vector>

using namespace std;

typedef int Vertex;
typedef vector<Vertex> VertexList;


class UtilityFunctions
{
    public:
        static void PrintVertexList(const VertexList &printList);
};

#endif 

