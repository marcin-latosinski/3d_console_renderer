#pragma once
#define _USE_MATH_DEFINES

#include "console_engine.hpp"
#include <cmath>

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

    void TranslateObject3D(object_3d* mesh)
    {
        std::vector<face_3d> translated_mesh;
        for (int face_idx = 0; face_idx < mesh->polygons.size(); face_idx++)
        {
            face_3d& face = mesh->polygons.at(face_idx);
            face_3d translated_face;
            for (int vec_idx = 0; vec_idx < N_VERTICES_IN_POLYGON; vec_idx++)
            {
                vec_3d& vec = face.vertex[vec_idx];
                translated_face.vertex[vec_idx] = VectMatrixMultiply(vec, mesh->trans_mat);
            }
            translated_mesh.push_back(translated_face);
        }
        mesh->polygons = translated_mesh;
    }

    void RotateObject3D(object_3d* mesh)
    {
        std::vector<face_3d> rotated_mesh;
        for (int face_idx = 0; face_idx < mesh->polygons.size(); face_idx++)
        {
            face_3d& face = mesh->polygons.at(face_idx);
            face_3d rotated_face;
            for (int vec_idx = 0; vec_idx < N_VERTICES_IN_POLYGON; vec_idx++)
            {
                vec_3d& vec = face.vertex[vec_idx];
                rotated_face.vertex[vec_idx] = VectMatrixMultiply(vec, mesh->rotat_mat);
            }
            rotated_mesh.push_back(rotated_face);
        }
        mesh->polygons = rotated_mesh;
    }

    void RenderObject3D(object_3d* mesh)
    {
        for (int face_idx = 0; face_idx < mesh->polygons.size(); face_idx++)
        {
            face_3d& face = mesh->polygons.at(face_idx);
            CalcFaceNormal(face);
            if ((face.normal.x * face.vertex[0].x +
                 face.normal.y * face.vertex[0].y +
                 face.normal.z * face.vertex[0].z ) > 0.0f)
            {
                projected_triangle projected;
                for (int vec_idx = 0; vec_idx < N_VERTICES_IN_POLYGON; vec_idx++)
                {
                    vec_3d& vec_in_3d = face.vertex[vec_idx];
                    projected.points[vec_idx] = Render3DPoint(vec_in_3d);
                }
                projected_mesh.push_back(projected);
            }
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

    void CalcFaceNormal(face_3d& in)
    {
        vec_3d line1, line2;

        line1.x = in.vertex[1].x - in.vertex[0].x;
        line1.y = in.vertex[1].y - in.vertex[0].y;
        line1.z = in.vertex[1].z - in.vertex[0].z;

        line2.x = in.vertex[2].x - in.vertex[0].x;
        line2.y = in.vertex[2].y - in.vertex[0].y;
        line2.z = in.vertex[2].z - in.vertex[0].z;

        in.normal.x = line1.y * line2.z - line1.z * line2.y;
        in.normal.y = line1.z * line2.x - line1.x * line2.z;
        in.normal.z = line1.x * line2.y - line1.y * line2.x;

        float length = std::sqrt(in.normal.x * in.normal.x + in.normal.y * in.normal.y + in.normal.z * in.normal.z);

        in.normal.x /= length;
        in.normal.y /= length;
        in.normal.z /= length;
    }

    vec_3d Translate3DPoint(vec_3d& in)
    {

    }

    vec_3d VectMatrixMultiply(vec_3d& in, matrix_4x4& mat)
    {
        vec_3d out;
        out.x   = in.x * proj_mat.c[0][0] + in.y * proj_mat.c[1][0] + in.z * proj_mat.c[2][0] + proj_mat.c[3][0];
        out.y   = in.x * proj_mat.c[0][1] + in.y * proj_mat.c[1][1] + in.z * proj_mat.c[2][1] + proj_mat.c[3][1];
        out.z   = in.x * proj_mat.c[0][2] + in.y * proj_mat.c[1][2] + in.z * proj_mat.c[2][2] + proj_mat.c[3][2];
        float w = in.x * proj_mat.c[0][3] + in.y * proj_mat.c[1][3] + in.z * proj_mat.c[2][3] + proj_mat.c[3][3];

        if (w > 0.f)
        {
            out.x /= w;
            out.y /= w;
            out.z /= w;
        }

        return out;
    }

    point_2d Render3DPoint(vec_3d& in)
    {


        //3D to 2D projection
        vec_3d projected = VectMatrixMultiply(in, proj_mat);

        //raster to screen
        projected.x = ( projected.x + 1.f ) * 0.5f * (float)width;
        projected.y = ( projected.y + 1.f ) * 0.5f * (float)height;

        point_2d out;
        out.x = (uint16_t)projected.x;
        out.y = (uint16_t)projected.y;
        out.z = projected.z;

        return out;
    }

    matrix_4x4 proj_mat;
    matrix_4x4 trans_mat;
    matrix_4x4 rot_mat;

    uint16_t width;
    uint16_t height;
    float fov;
    float aspect_ratio;
    float z_clip_near;
    float z_clip_far;

    object_3d cube;

    std::vector<projected_triangle> projected_mesh;
};
