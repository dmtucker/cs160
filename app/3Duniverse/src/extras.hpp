#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078
#define DEG2RAD (PI/180.0)
#define cot(x) tan((PI/2)-x)

double linear_map ( const double x , const double min , const double max , const double new_min , const double new_max ) {
    return (((x-min)*(new_max-new_min))/(max-min))+new_min;
}

double percentage ( double x , double min , double max ) {
    return linear_map(x,min,max,0,100);
}

void printVec3 ( vec3 v ) { printf("(%6.3f,%6.3f,%6.3f)\n"      ,v.x,v.y,v.z); }
void printVec4 ( vec4 v ) { printf("(%6.3f,%6.3f,%6.3f,%6.3f)\n",v.x,v.y,v.z,v.w); }
void printMat4 ( mat4 v ) { printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[0].x,v[0].y,v[0].z,v[0].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[1].x,v[1].y,v[1].z,v[1].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[2].x,v[2].y,v[2].z,v[2].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",v[3].x,v[3].y,v[3].z,v[3].w); }

