#include "io_test_helper.hpp"

READ_TEST("3mf", "test_3mf/Geräte/box.3mf", Slic3r::IO::TMF::read, box_10x20x30)
WRITE_TEST("3mf", Slic3r::IO::TMF::read, Slic3r::IO::TMF::write, box_10x20x30)
