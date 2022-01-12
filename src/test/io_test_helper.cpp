#include "io_test_helper.hpp"

#include <cstdio>

namespace Slic3r { namespace Test {

std::vector<test_triangle>
to_triangles(Slic3r::TriangleMesh* mesh)
{
    Slic3r::Pointf3s vertices = mesh->vertices();
    Slic3r::Point3s facets = mesh->facets();
    std::vector<test_triangle> result;
    result.reserve(facets.size());
    for (Slic3r::Point3 facet : facets) {
        test_triangle tri = { 
            vertices[facet.x], 
            vertices[facet.y], 
            vertices[facet.z] 
        };
        result.push_back(tri);
    }
    return result;
}

void
print_mesh(Slic3r::TriangleMesh* mesh)
{
    Slic3r::Pointf3s vertices = mesh->vertices();
    Slic3r::Point3s facets = mesh->facets();
    for (Slic3r::Pointf3 v : vertices) {
        std::cout << "Pointf3(" << v.x << ", " << v.y << ",  " << v.z << "),\n";
    }
    std::cout << "\n\n";
    for (Slic3r::Point3 f : facets) {
        std::cout << "Point3(" << f.x << ", " << f.y << ",  " << f.z << "),\n";
    }
}

template<class T>
bool
matches_pairs(const std::vector<T> items, std::vector<T> available, bool(*match_fcn)(const T*, const T*)) 
{
    // Items must match one-to-one
    for (int i = 0; i < items.size(); i++) {
        int j;
        bool matched = false;
        for (j = 0; j != available.size(); j++) {
            if(match_fcn(&items[i], &available[j])) {
                matched = true;
                break;
            }
        }
        // Matched vertex no longer available
        if (matched) { 
            available.erase(available.begin() + j);
        }
        else {
            return false;
        }
    }
    return available.size() == 0;
}

bool
vertices_equal(const Slic3r::Pointf3* v1, const Slic3r::Pointf3* v2) {
    return v1->distance_to(*v2) < EPSILON;
}

bool
triangles_equal(const test_triangle* t1, const test_triangle* t2)
{
    std::vector<Pointf3> points(t1->vertex, t1->vertex + 3);
    std::vector<Pointf3> remaining(t2->vertex, t2->vertex + 3);
    return matches_pairs<Slic3r::Pointf3>(points, remaining, &vertices_equal);
}

bool
meshes_equal(Slic3r::TriangleMesh* m1, Slic3r::TriangleMesh* m2) 
{
    const std::vector<test_triangle> triangles1 = to_triangles(m1);
    const std::vector<test_triangle> triangles2 = to_triangles(m2);
    return matches_pairs<test_triangle>(triangles1, triangles2, &triangles_equal);
}

} } // namespace Slic3r namespace Test
