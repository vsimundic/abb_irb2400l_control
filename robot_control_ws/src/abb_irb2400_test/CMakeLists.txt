cmake_minimum_required(VERSION 3.0.2)
project(abb_irb2400_test)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cmake_modules
  interactive_markers
  moveit_core
  moveit_ros_perception
  moveit_ros_planning_interface
  moveit_visual_tools
  pluginlib
  roscpp
  std_msgs
  roi_msgs
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
  # roi_msgs
)


###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 INCLUDE_DIRS include
 LIBRARIES RobotControl
 CATKIN_DEPENDS cmake_modules interactive_markers moveit_core moveit_ros_perception moveit_ros_planning_interface pluginlib roscpp std_msgs roi_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_library(RobotControl src/robotcontrol.cpp)
target_link_libraries(RobotControl ${catkin_LIBRARIES})

add_executable(RobotControl_node src/robotcontrol_node.cpp)
target_link_libraries(RobotControl_node ${catkin_LIBRARIES} RobotControl)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
add_executable(${PROJECT_NAME}_random_node src/test_random.cpp)
add_executable(${PROJECT_NAME}_custom_node src/test_custom.cpp)


## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(${PROJECT_NAME}_custom_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_random_node ${PROJECT_NAME}_generate_messages_cpp)
add_dependencies(${PROJECT_NAME}_custom_node ${PROJECT_NAME}_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(${PROJECT_NAME}_random_node ${catkin_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_custom_node ${catkin_LIBRARIES})
