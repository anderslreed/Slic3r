#include "io_test_macros.hpp"


READ_TEST("stl", "test_cli/20mmbox.stl", STL, box_20x20x10_centered)
WRITE_TEST("stl", STL, cube_20x20x20)
