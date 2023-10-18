


#include <vector>
#include <cmath> 
#include <cassert>
#include <functional>
#include <algorithm>
#include <map>
#include <memory>
#include <functional>

// Types
typedef float lfloat;  

using point = std::vector<lfloat>;
using dissimilarities = std::vector<std::vector<lfloat>>;

struct claster{
    claster(const point& pnt){
        points.push_back(pnt);
    }
    std::vector<point> points;
};

//

#include <iostream>

#include "converter.h" 
#include "generic_lincage.h"
