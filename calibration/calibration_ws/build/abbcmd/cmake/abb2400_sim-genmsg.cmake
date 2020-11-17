# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "abb2400_sim: 0 messages, 1 services")

set(MSG_I_FLAGS "-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(abb2400_sim_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" NAME_WE)
add_custom_target(_abb2400_sim_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "abb2400_sim" "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" "geometry_msgs/Quaternion:geometry_msgs/Point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages

### Generating Services
_generate_srv_cpp(abb2400_sim
  "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/abb2400_sim
)

### Generating Module File
_generate_module_cpp(abb2400_sim
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/abb2400_sim
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(abb2400_sim_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(abb2400_sim_generate_messages abb2400_sim_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" NAME_WE)
add_dependencies(abb2400_sim_generate_messages_cpp _abb2400_sim_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(abb2400_sim_gencpp)
add_dependencies(abb2400_sim_gencpp abb2400_sim_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS abb2400_sim_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages

### Generating Services
_generate_srv_eus(abb2400_sim
  "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/abb2400_sim
)

### Generating Module File
_generate_module_eus(abb2400_sim
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/abb2400_sim
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(abb2400_sim_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(abb2400_sim_generate_messages abb2400_sim_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" NAME_WE)
add_dependencies(abb2400_sim_generate_messages_eus _abb2400_sim_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(abb2400_sim_geneus)
add_dependencies(abb2400_sim_geneus abb2400_sim_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS abb2400_sim_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages

### Generating Services
_generate_srv_lisp(abb2400_sim
  "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/abb2400_sim
)

### Generating Module File
_generate_module_lisp(abb2400_sim
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/abb2400_sim
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(abb2400_sim_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(abb2400_sim_generate_messages abb2400_sim_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" NAME_WE)
add_dependencies(abb2400_sim_generate_messages_lisp _abb2400_sim_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(abb2400_sim_genlisp)
add_dependencies(abb2400_sim_genlisp abb2400_sim_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS abb2400_sim_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages

### Generating Services
_generate_srv_nodejs(abb2400_sim
  "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/abb2400_sim
)

### Generating Module File
_generate_module_nodejs(abb2400_sim
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/abb2400_sim
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(abb2400_sim_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(abb2400_sim_generate_messages abb2400_sim_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" NAME_WE)
add_dependencies(abb2400_sim_generate_messages_nodejs _abb2400_sim_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(abb2400_sim_gennodejs)
add_dependencies(abb2400_sim_gennodejs abb2400_sim_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS abb2400_sim_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages

### Generating Services
_generate_srv_py(abb2400_sim
  "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/abb2400_sim
)

### Generating Module File
_generate_module_py(abb2400_sim
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/abb2400_sim
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(abb2400_sim_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(abb2400_sim_generate_messages abb2400_sim_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/valentin/FAKS/HDR/robot_workspaces/calibration/calibration_ws/src/abbcmd/srv/move_req.srv" NAME_WE)
add_dependencies(abb2400_sim_generate_messages_py _abb2400_sim_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(abb2400_sim_genpy)
add_dependencies(abb2400_sim_genpy abb2400_sim_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS abb2400_sim_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/abb2400_sim)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/abb2400_sim
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(abb2400_sim_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(abb2400_sim_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/abb2400_sim)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/abb2400_sim
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(abb2400_sim_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(abb2400_sim_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/abb2400_sim)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/abb2400_sim
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(abb2400_sim_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(abb2400_sim_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/abb2400_sim)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/abb2400_sim
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(abb2400_sim_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(abb2400_sim_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/abb2400_sim)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/abb2400_sim\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/abb2400_sim
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(abb2400_sim_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(abb2400_sim_generate_messages_py geometry_msgs_generate_messages_py)
endif()
