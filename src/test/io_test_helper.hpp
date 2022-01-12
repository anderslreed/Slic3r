#ifndef SLIC3R_TEST_HELPER_HPP
#define SLIC3R_TEST_HELPER_HPP

#include <boost/filesystem.hpp>
#include <catch.hpp>

#include "IO.hpp"
#include "Model.hpp"
#include "test_data.hpp"
#include "test_options.hpp"
#include "TriangleMesh.hpp"

// The guts of a read/verify test
#define _READ_TEST(NAME, FILEPATH, READ_FCN, MODEL, MESH_NAME) \
    int res = READ_FCN(testfile(FILEPATH), MODEL); \
    THEN("read should not return 0") { \
        REQUIRE(res == 1); \
    } \
    THEN("Model contains one ModelObject.") { \
        REQUIRE(read_model->objects.size() == 1); \
    } \
    THEN("ModelObject contains one ModelVolume") { \
        REQUIRE(read_model->objects[0]->volumes.size() == 1); \
    } \
    THEN("resulting mesh is correct") { \
        Slic3r::TriangleMesh expected = Slic3r::Test::mesh(Slic3r::Test::TestMesh::MESH_NAME); \
        TriangleMesh result = read_model->objects[0]->volumes[0]->mesh; \
        REQUIRE(result.facets().size() > 0); \
        REQUIRE(Slic3r::Test::meshes_equal(&result, &expected)); \
    }

// Read from FILEPATH using READ_FCN, and compare the resulting mesh to the mesh specified by MESH_NAME
#define READ_TEST(NAME, FILEPATH, READ_FCN, MESH_NAME) \
    SCENARIO("Reading "  NAME " file") { \
        Slic3r::Model* read_model = new Slic3r::Model(); \
        GIVEN("Input file " FILEPATH ", Expected mesh " #MESH_NAME) { \
            WHEN(NAME " model is read") { \  
                _READ_TEST(NAME, FILEPATH, READ_FCN, read_model, MESH_NAME) \
            } \
        } \
    }

// Write the mesh specified by mesh_name to a temp file, then read that file and check the result
#define WRITE_TEST(NAME, READ_FCN, WRITE_FCN, MESH_NAME) \
    SCENARIO("Writing " NAME " file") { \
        boost::filesystem::create_directory(testfile("tmp")); \
        Slic3r::Model* write_model = new Model(); \
        Slic3r::Model* read_model = new Model(); \
        ModelObject* obj = write_model->add_object(); \
        obj->add_volume(Slic3r::Test::mesh(Slic3r::Test::TestMesh::MESH_NAME)); \
        obj->add_instance(); \
        GIVEN("Mesh " #MESH_NAME) { \         
            WHEN(NAME " model is read and written as temp file") { \
                bool ret = WRITE_FCN(*write_model, testfile("tmp/target")); \
                THEN("write should not return 0") { \
                    REQUIRE(ret == true); \
                } \ 
                _READ_TEST(NAME, "tmp/target", READ_FCN, read_model, MESH_NAME) \
            } \
        } \
        boost::filesystem::remove(testfile("tmp/target")); \
    }

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