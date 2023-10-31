# If ROOT is not installed in a default system location you need to tell CMake where to find it.
# Sourcing `thisroot.sh` already sets the required environment variables.
# Otherwise, you must tell the build system where to look for ROOT,
# for example by passing `-DROOT_DIR="/path/to/root/installation` at CMake configuration time.

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(rplot)

# pass root folder to build flags (root-framework installation under ${HOME}/root/)
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-DROOT_DIR=$ENV{HOME}/root/cmake/;>")

find_package(catkin REQUIRED COMPONENTS roscpp roslib)
# Locate the ROOT package and define a number of useful targets and variables.
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Imt Graf Hist Graf3d Gpad ROOTDataFrame MathCore Rint Matrix Physics ROOTVecOps Gui)

catkin_package(
  INCLUDE_DIRS      include
  CATKIN_DEPENDS    roscpp
                    roslib
  LIBRARIES         rplot
  DEPENDS           ROOT
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${catkin_INCLUDE_DIRS}
                    ${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARIES})
# set build flags (-std=c++14 for root v6.28.06, ignore warnings from other packages including this)
list(APPEND CMAKE_CXX_FLAGS "${ROOT_CXX_FLAGS}")

# Create a shared library.
# Passing cmake targets such as `ROOT::RIO` as dependencies (rather than plain
# library names for example via ${ROOT_LIBRARIES}) ensures that properties such as required
# include directories and C++ standard are propagated to our libraries or executables.
# Note: To ensure compatibility with Cling, targets *must* be compiled using the
# same C++ standard as ROOT was compiled with.
add_library(${PROJECT_NAME} src/rplot.cpp)
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${catkin_LIBRARIES} ${ROOT_LIBRARIES})#ROOT::Core ROOT::RIO ROOT::Net ROOT::Gui ROOT::Graf ROOT::Gpad ROOT::Imt ROOT::Hist ROOT::Graf3d ROOT::Gpad ROOT::ROOTDataFrame ROOT::MathCore ROOT::Rint ROOT::Matrix ROOT::Physics ROOT::ROOTVecOps)

# Create the main program using the library.
add_executable(main src/main.cpp)
target_link_libraries(main ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
