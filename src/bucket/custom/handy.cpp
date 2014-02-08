void vec3print ( vec3 v ) { printf("(%6.3f,%6.3f,%6.3f)\n",v.x,v.y,v.z); }

void mat3print ( mat3 m ) { printf("|%6.3f,%6.3f,%6.3f|\n",m[0].x,m[0].y,m[0].z);
                            printf("|%6.3f,%6.3f,%6.3f|\n",m[1].x,m[1].y,m[1].z);
                            printf("|%6.3f,%6.3f,%6.3f|\n",m[2].x,m[2].y,m[2].z); }

void vec4print ( vec4 v ) { printf("(%6.3f,%6.3f,%6.3f,%6.3f)\n",v.x,v.y,v.z,v.w); }

void mat4print ( mat4 m ) { printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",m[0].x,m[0].y,m[0].z,m[0].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",m[1].x,m[1].y,m[1].z,m[1].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",m[2].x,m[2].y,m[2].z,m[2].w);
                            printf("|%6.3f,%6.3f,%6.3f,%6.3f|\n",m[3].x,m[3].y,m[3].z,m[3].w); }
