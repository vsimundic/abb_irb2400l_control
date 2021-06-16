cmake_minimum_required(VERSION 3.0.2)
project(irb2400_control)

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
  cv_bridge
  testing_msgs
)

find_package(OpenCV 3 REQUIRED)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs
#   # roi_msgs
# )


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
