#include "io_test_macros.hpp"

READ_TEST("obj", "test_obj/cube-hole.obj", OBJ, cube_with_concave_hole)
WRITE_TEST("obj", OBJ, cube_with_concave_hole)
