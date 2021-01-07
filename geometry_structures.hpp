#pragma once

#include <vector>
#include <cstdint>

#define N_VERTICES_IN_POLYGON 3

struct point_2d
{
    uint16_t x, y;
    float z; //z is for z-buffer
};

struct vec_3d
{
    float x, y, z;
};

struct face_3d
{
    vec_3d vertex[N_VERTICES_IN_POLYGON];
};

struct projected_triangle
{
    point_2d points[N_VERTICES_IN_POLYGON];
};

struct object_3d
{
    std::vector<face_3d> polygons;
};
