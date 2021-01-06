#pragma once

#include "engine.hpp"
#include <cmath>

constexpr float PI() { return std::atanhf(1.0f) * 4.0f; }

struct matrix_4x4
{
    float c[4][4] = { 0.f };
};

struct vect_homo
{
    float x, y, z, w = { 0.f };
};

struct vect_cart
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

    matrix_4x4 proj_mat;
    float fov;
    float aspect_ratio;
    float z_clip_near;
    float z_clip_far;
};