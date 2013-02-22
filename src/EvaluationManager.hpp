#ifndef __EVALUATION_MANAGER_HPP__
#define __EVALUATION_MANAGER_HPP__

//#include "boost/tuple/tuple.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <climits>
#include <queue>
#define foreach BOOST_FOREACH


class EvaluationManager{
    public:
        EvaluationManager();
        virtual ~EvaluationManager();
};

#endif
