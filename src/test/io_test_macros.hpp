#include <boost/filesystem.hpp>
#include <catch.hpp>

#include "IO.hpp"
#include "Model.hpp"
#include "test_data.hpp"
#include "io_test_helper.hpp"


// The guts of a read/verify test
#define _READ_TEST(NAME, FILEPATH, IO_NAME, MODEL, MESH_NAME) \
    int res = Slic3r::IO::IO_NAME::read(testfile(FILEPATH), MODEL); \
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
        result.repair(); \
        REQUIRE(result.facets().size() > 0); \
        REQUIRE(Slic3r::Test::meshes_equal(&result, &expected)); \
    }

// Read from FILEPATH using READ_FCN, and compare the resulting mesh to the mesh specified by MESH_NAME
#define READ_TEST(NAME, FILEPATH, IO_NAME, MESH_NAME) \
    SCENARIO("Reading "  NAME " file") { \
        Slic3r::Model* read_model = new Slic3r::Model(); \
        GIVEN("Input file " FILEPATH ", Expected mesh " #MESH_NAME) { \
            WHEN(NAME " model is read") { \  
                _READ_TEST(NAME, FILEPATH, IO_NAME, read_model, MESH_NAME) \
            } \
        } \
    }

// Write the mesh specified by mesh_name to a temp file, then read that file and check the result
#define WRITE_TEST(NAME, IO_NAME, MESH_NAME) \
    SCENARIO("Writing " NAME " file") { \
        boost::filesystem::create_directory(testfile("tmp")); \
        Slic3r::Model* write_model = new Model(); \
        Slic3r::Model* read_model = new Model(); \
        ModelObject* obj = write_model->add_object(); \
        obj->add_volume(Slic3r::Test::mesh(Slic3r::Test::TestMesh::MESH_NAME)); \
        obj->add_instance(); \
        GIVEN("Mesh " #MESH_NAME) { \         
            WHEN(NAME " model is read and written as temp file") { \
                bool ret = Slic3r::IO::IO_NAME::write(*write_model, testfile("tmp/target")); \
                THEN("write should not return 0") { \
                    REQUIRE(ret == true); \
                } \ 
                _READ_TEST(NAME, "tmp/target", IO_NAME, read_model, MESH_NAME) \
            } \
        } \
        boost::filesystem::remove(testfile("tmp/target")); \
    }
