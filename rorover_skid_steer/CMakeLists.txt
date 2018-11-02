#------ package and version details ------------------
cmake_minimum_required(VERSION 2.8.3)
project(rorover_skid_steer)

#------ define dependencies --------------------------

find_package(catkin REQUIRED
  std_msgs
  geometry_msgs
  sensor_msgs
  roscpp
  rospy
)

#------ define C++ nodes ------------------------------
catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(rover_teleop src/teleop.cpp)
target_link_libraries(rover_teleop ${catkin_LIBRARIES} )

#------ define installation ---------------------------

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY meshes
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY urdf
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY rviz
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
