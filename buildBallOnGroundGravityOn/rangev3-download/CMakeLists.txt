cmake_minimum_required(VERSION 3.2)

project(rangev3-download NONE)

include(ExternalProject)
ExternalProject_Add(rangev3
  GIT_REPOSITORY    https://github.com/ericniebler/range-v3.git
  GIT_TAG           master
  SOURCE_DIR        "/home/taharabun/Projects/TestMujocoPhysics/buildBallOnGroundGravityOn/rangev3-src"
  BINARY_DIR        ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
