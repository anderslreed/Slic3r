#include <catch.hpp>
#include <test_options.hpp>
#include "Model.hpp"
#include "IO.hpp"
#include "io_test_helper.hpp"


using namespace Slic3r;
using namespace std::literals::string_literals;

READ_TEST("Compressed AMF file of a 20mm cube", "test_amf/20mmbox_deflated.amf", AMF, box_20x20x10_centered)
READ_TEST("Compressed AMF file of a 20mm cube with some subdirectories", "test_amf/20mmbox_deflated-in_directories.amf", AMF, box_20x20x10_centered)
READ_TEST("Compressed AMF file of a 20mm cube with multiple files in the archive", "test_amf/20mmbox_deflated-mult_files.amf", AMF, box_20x20x10_centered)
READ_TEST("Uncompressed AMF file of a 20mm cube", "test_amf/20mmbox.amf", AMF, box_20x20x10_centered)

SCENARIO("Uncompressed AMF file of a 20mm cube") {
    auto model {new Slic3r::Model()};
    GIVEN("No file") {
        WHEN("nonexistant file is read") {
            bool result_code = Slic3r::IO::AMF::read(testfile("test_amf/20mmbox-doesnotexist.amf"), model);;
            THEN("Read returns false.") {
                REQUIRE(result_code == false);
            }
            THEN("Model object contains no ModelObject.") {
                REQUIRE(model->objects.size() == 0);
            }
        }
    }
}

SCENARIO("Reading AMF file", "[AMF]") {
    GIVEN("badly formed AMF file (missing vertices)") {
        auto model {new Slic3r::Model()};
        WHEN("AMF model is read") {
            auto ret = Slic3r::IO::AMF::read(testfile("test_amf/5061-malicious.amf"),model);
            THEN("read should return True") {
                REQUIRE(ret);
            }
        }
    }
    GIVEN("Ok formed AMF file") {
        auto model {new Slic3r::Model()};
        WHEN("AMF model is read") {
            auto ret = Slic3r::IO::AMF::read(testfile("test_amf/read-amf.amf"),model);
            THEN("read should return True") {
                REQUIRE(ret);
            }
        }
    }
}
