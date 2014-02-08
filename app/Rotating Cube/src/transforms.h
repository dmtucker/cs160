#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stdlib.h>

double linear_map ( const double x , const double min , const double max , const double new_min , const double new_max ) {
    return (((x-min)*(new_max-new_min))/(max-min))+new_min;
}

double percentage ( double x , double min , double max ) {
    return linear_map(x,min,max,0,100);
}

#endif
