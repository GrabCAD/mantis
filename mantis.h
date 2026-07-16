#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <array>

namespace mantis {

enum PrimitiveType {
    Vertex = 0,
    Edge = 1,
    Face = 2
};

struct Result {
    float distance_squared = -1.f;
    int primitive_index = 0;
    float closest_point[3] = {};
    PrimitiveType type{};
};

struct Impl;

struct AccelerationStructure {
    /* Build from a strided vertex array and packed triangle index array.
     * points is interpreted as: vertices[i] = {points[i*stride+0], points[i*stride+1], points[i*stride+2]}.
     * indices is a flat array of 3 uint32_t per face (indices[3*f+i] = vertex id).
     * A default stride of 3 covers the common packed-layout case. */
    AccelerationStructure(const float *points, size_t num_points, const uint32_t *indices, size_t num_faces,
                   float limit_cube_len = 1e3f, size_t vertex_stride = 3);

    // no copying
    AccelerationStructure(const AccelerationStructure&) = delete;
    AccelerationStructure& operator=(const AccelerationStructure&) = delete;

    AccelerationStructure(AccelerationStructure&&) noexcept;
    AccelerationStructure& operator=(AccelerationStructure&&) noexcept;

    /* Compute closest point on the mesh to (x, y, z). */
    Result calc_closest_point(float x, float y, float z) const;

    /* Compute closest point on the mesh to *q.
     * q must point to at least 3 consecutive floats {x, y, z}. */
    Result calc_closest_point(const float q[3]) const;

    size_t num_edges() const;
    size_t num_faces() const;
    size_t num_vertices() const;

    // For each face returns the indices of the three edges bordering
    // the face
    std::vector<std::array<uint32_t, 3>> get_face_edges() const;

    // For each edge returns the indices of the two vertices that make up the edge
    std::vector<std::pair<uint32_t, uint32_t>> get_edge_vertices() const;

    // For each face returns the indices of the three vertices that make up the face
    std::vector<std::array<uint32_t, 3>> get_faces() const;

    // Returns the positions of the vertices
    std::vector<std::array<float, 3>> get_positions() const;

    std::pair<uint32_t, uint32_t> get_edge(size_t index) const;

    ~AccelerationStructure();

    Impl *impl = nullptr;
};

}