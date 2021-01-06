#pragma once

#include <vector>
#include <cstdint>

#define N_VERTICES_IN_POLYGON 3

struct point_2d
{
    uint16_t x, y;
};

struct vertex_3d
{
    float x, y, z;
};

struct face_3d
{
    vertex_3d vertex[N_VERTICES_IN_POLYGON] = { 0.f, 0.f, 0.f };
};

struct object_3d
{
    std::vector<face_3d> polygons;
};
