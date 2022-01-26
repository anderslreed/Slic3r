#ifndef SLIC3R_TEST_HELPER_HPP
#define SLIC3R_TEST_HELPER_HPP

#include "Model.hpp"
#include "test_options.hpp"
#include "TriangleMesh.hpp"


namespace Slic3r { namespace Test {

typedef struct {
    Pointf3 vertex[3];
} test_triangle; 

std::vector<test_triangle> to_triangles(const Slic3r::TriangleMesh m);

void print_mesh(Slic3r::TriangleMesh* mesh);

template<class T>
bool matches_pairs(const std::vector<T> items, std::vector<T> available, bool(*match_fcn)(const T*, const T*));

bool vertices_equal(const Slic3r::Pointf3* v1, const Slic3r::Pointf3* v2);

bool triangles_equal(const test_triangle* t1, const test_triangle* t2);

bool meshes_equal(Slic3r::TriangleMesh* m1, Slic3r::TriangleMesh* m2);

} } // namespace Slic3r namespace Test

#endif // SLIC3R_TEST_HELPER_HPP