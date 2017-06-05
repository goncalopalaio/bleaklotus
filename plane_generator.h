#ifndef PLANE_GENERATOR_H
#define PLANE_GENERATOR_H

    #include "graphics_libs.h"
    #include "maratis_math.h"

    #define PLANE_N_SQUARES 100
    #define PLANE_SIDE_SIZE 1100


    void influence_plane_points(
        float domain,
        float3* a,
        float3* b,
        float3* c,
        float3* d,
        float3* e,
        float3* f);

    float sample_plane_height(float x, float z, float max);
    void load_plane(GLuint* plane_vao, int* point_count);

#endif

