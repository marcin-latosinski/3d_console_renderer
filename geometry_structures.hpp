#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <cstdint>
#include <cstring>
#include <cmath>

#define N_VERTICES_IN_POLYGON 3

struct matrix_4x4
{
    float c[4][4] = { 0.f };
};

struct point_2d
{
    uint16_t x, y;
    float z; //z is for z-buffer
};

struct projected_triangle
{
    point_2d points[N_VERTICES_IN_POLYGON];
};

struct vec_3d
{
    float x, y, z;
};

struct face_3d
{
    vec_3d vertex[N_VERTICES_IN_POLYGON];
    vec_3d normal;
};

struct object_3d
{
    std::vector<face_3d> polygons;
    matrix_4x4 trans_mat;
    matrix_4x4 rotat_mat;

    void rotate_z(float angle)
    {
        rotat_mat.c[0][0] = std::cos(angle);
        rotat_mat.c[0][1] = std::sin(angle);
        rotat_mat.c[1][0] = -std::sin(angle);
        rotat_mat.c[1][1] = std::cos(angle);
        rotat_mat.c[2][2] = 1;
        rotat_mat.c[3][3] = 1;
    }
    void rotate_y(float angle)
    {
		rotat_mat.c[0][0] = std::cos(angle);
		rotat_mat.c[1][1] = 1;
		rotat_mat.c[1][2] = -std::sin(angle);
		rotat_mat.c[2][1] = std::sin(angle);
		rotat_mat.c[2][2] = std::cos(angle);
		rotat_mat.c[3][3] = 1;
    }

    void translate(float x, float y)
    {
        trans_mat.c[0][0] = 1.0f;
		trans_mat.c[1][1] = 1.0f;
		trans_mat.c[2][2] = 1.0f;
		trans_mat.c[3][3] = 1.0f;
		trans_mat.c[3][0] = x;
		trans_mat.c[3][1] = y;
		trans_mat.c[3][2] = 0.0f;
    }

    void reset_matrices()
    {
        std::memset(&trans_mat, 0, sizeof(trans_mat));
        std::memset(&rotat_mat, 0, sizeof(rotat_mat));
    }
};
