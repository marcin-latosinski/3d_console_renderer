#pragma once

#include "console_engine.hpp"
#include <cmath>

constexpr float PI() { return std::atanhf(1.0f) * 4.0f; }

struct matrix_4x4
{
    float c[4][4] = { 0.f };
};

struct vec_homo
{
    float x, y, z, w = { 0.f };
};

struct vec_cart
{
    float x, y, z = { 0.f };
};

class ProjectionEngine
{
public:
    ProjectionEngine()
    {
        uint16_t width, height;
        ConsoleEngine::GetScreenSize(width, height);

        fov = 90.f;
        aspect_ratio = height/width;
        z_clip_near = 1.f;
        z_clip_far  = 100.f;

        SetupProjectionMatrix();

    }

    ~ProjectionEngine()
    {

    }

private:

    void SetupProjectionMatrix()
    {
        proj_mat.c[0][0] = 1.f / tanf((fov/2.f) * PI()/180.f);
        proj_mat.c[1][1] = 1.f / tanf((fov/2.f) * PI()/180.f);
        proj_mat.c[2][2] = (-1.f * z_clip_far) / (z_clip_far - z_clip_near);
        proj_mat.c[3][2] = (-1.f * z_clip_far * z_clip_near) / (z_clip_far - z_clip_near);
        proj_mat.c[2][3] = (-1.f);
    }

    vec_cart VecProjMatrixMultiplication(vec_homo& in)
    {
        vec_cart out_mat;
        out_mat.x   = in.x * proj_mat.c[0][0] + in.y * proj_mat.c[1][0] + in.z * proj_mat.c[2][0] + proj_mat.c[3][0]; 
        out_mat.y   = in.x * proj_mat.c[0][1] + in.y * proj_mat.c[1][1] + in.z * proj_mat.c[2][1] + proj_mat.c[3][1]; 
        out_mat.z   = in.x * proj_mat.c[0][2] + in.y * proj_mat.c[1][2] + in.z * proj_mat.c[2][2] + proj_mat.c[3][2]; 
        float w     = in.x * proj_mat.c[0][3] + in.y * proj_mat.c[1][3] + in.z * proj_mat.c[2][3] + proj_mat.c[3][3]; 

        if (w < 1.f && w > 0.f)
        { 
            out_mat.x /= w; 
            out_mat.y /= w; 
            out_mat.z /= w; 
        } 
    }

    matrix_4x4 proj_mat;
    float fov;
    float aspect_ratio;
    float z_clip_near;
    float z_clip_far;
};