#pragma once
#define _USE_MATH_DEFINES

#include "console_engine.hpp"
#include <cmath>

struct matrix_4x4
{
    float c[4][4] = { 0.f };
};

class ProjectionEngine
{
public:
    ProjectionEngine()
    {
        ConsoleEngine::GetScreenSize(width, height);

        fov = 90.0f;
        aspect_ratio = (float)height / (float)width;
        z_clip_near = 0.1f;
        z_clip_far  = 100.0f;

        // STATIC DEFINED TEST 3D MESH
        cube.polygons = {// SOUTH
                        {{ {0.0f, 0.0f, -18.0f},  {0.0f, 10.0f, -18.0f},  {10.0f, 10.0f, -18.0f} }},
                        {{ {0.0f, 0.0f, -18.0f},  {10.0f, 10.0f, -18.0f},  {10.0f, 0.0f, -18.0f} }},
                         // EAST
                        {{ {10.0f, 0.0f, -18.0f},  {10.0f, 10.0f, -18.0f},  {10.0f, 10.0f, -28.0f} }},
                        {{ {10.0f, 0.0f, -18.0f},  {10.0f, 10.0f, -28.0f},  {10.0f, 0.0f, -28.0f} }},
                         // NORTH
                        {{ {10.0f, 0.0f, -28.0f},  {10.0f, 10.0f, -28.0f},  {0.0f, 10.0f, -28.0f} }},
                        {{ {10.0f, 0.0f, -28.0f},  {0.0f, 10.0f, -28.0f},  {0.0f, 0.0f, -28.0f} }},
                         // WEST
                        {{ {0.0f, 0.0f, -28.0f},  {0.0f, 10.0f, -28.0f},  {0.0f, 10.0f, -18.0f} }},
                        {{ {0.0f, 0.0f, -28.0f},  {0.0f, 10.0f, -18.0f},  {0.0f, 0.0f, -18.0f} }},
                         // TOP
                        {{ {0.0f, 10.0f, -18.0f},  {0.0f, 10.0f, -28.0f},  {10.0f, 10.0f, -28.0f} }},
                        {{ {0.0f, 10.0f, -18.0f},  {10.0f, 10.0f, -28.0f},  {10.0f, 10.0f, -18.0f} }},
                         // BOTTOM
                        {{ {10.0f, 0.0f, -28.0f},  {0.0f, 0.0f, -28.0f},  {0.0f, 0.0f, -18.0f} }},
                        {{ {10.0f, 0.0f, -28.0f},  {0.0f, 0.0f, -18.0f},  {10.0f, 0.0f, -18.0f} }} };

        SetupProjectionMatrix();

    }

    object_3d* GetCubePtr()
    {
        return &cube;
    }

    std::vector<projected_triangle>* GetProjectedMeshPtr()
    {
        return &projected_mesh;
    }

    void ProjectObject3D(object_3d* mesh)
    {
        for (int face_idx = 0; face_idx < mesh->polygons.size(); face_idx++)
        {
            face_3d& face = mesh->polygons.at(face_idx);
            projected_triangle projected;
            for (int vec_idx = 0; vec_idx < N_VERTICES_IN_POLYGON; vec_idx++)
            {
                vec_3d& vec_in_3d = face.vertex[vec_idx];
                projected.points[vec_idx] = Project3DPoint(vec_in_3d);
            }
            projected_mesh.push_back(projected);
        }
    }

    ~ProjectionEngine()
    {

    }

private:

    void SetupProjectionMatrix()
    {
        proj_mat.c[0][0] = (1.0f / std::tan((fov/2.0f) * M_PI/180.0f)) * aspect_ratio;
        proj_mat.c[1][1] = 1.0f / std::tan((fov/2.0f) * M_PI/180.0f);
        proj_mat.c[2][2] = (-1.0f * z_clip_far) / (z_clip_far - z_clip_near);
        proj_mat.c[3][2] = (-1.0f * z_clip_far * z_clip_near) / (z_clip_far - z_clip_near);
        proj_mat.c[2][3] = (-1.0f);
    }

    point_2d Project3DPoint(vec_3d& in)
    {
        point_2d out;
        float x = in.x * proj_mat.c[0][0] + in.y * proj_mat.c[1][0] + in.z * proj_mat.c[2][0] + proj_mat.c[3][0];
        float y = in.x * proj_mat.c[0][1] + in.y * proj_mat.c[1][1] + in.z * proj_mat.c[2][1] + proj_mat.c[3][1];
        float z = in.x * proj_mat.c[0][2] + in.y * proj_mat.c[1][2] + in.z * proj_mat.c[2][2] + proj_mat.c[3][2];
        float w = in.x * proj_mat.c[0][3] + in.y * proj_mat.c[1][3] + in.z * proj_mat.c[2][3] + proj_mat.c[3][3];

        //raster to screen
        x = ( x + 1.f ) * 0.5f * (float)width;
        y = ( y + 1.f ) * 0.5f * (float)height;

        if (w > 0.f)
        {
            x /= w;
            y /= w;
            z /= w;
        }

        out.x = (uint16_t)x;
        out.y = (uint16_t)y;
        out.z = z;

        return out;
    }

    matrix_4x4 proj_mat;

    uint16_t width;
    uint16_t height;
    float fov;
    float aspect_ratio;
    float z_clip_near;
    float z_clip_far;

    object_3d cube;

    std::vector<projected_triangle> projected_mesh;
};
