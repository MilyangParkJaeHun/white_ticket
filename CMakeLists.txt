cmake_minimum_required(VERSION 2.8.3)
project(white_ticket)

find_package(catkin REQUIRED COMPONENTS
	message_generation
  roscpp
  rospy
  std_msgs
)

find_library(wiringPi_LIB wiringPi)

add_message_files(
	FILES Distance.msg
	FILES Seed.msg
)

generate_messages(
	DEPENDENCIES
	std_msgs
)

catkin_package(
  LIBRARIES white_ticket
  CATKIN_DEPENDS roscpp rospy std_msgs
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(ultra_sonic src/ultra_sonic.cpp)
add_dependencies(ultra_sonic ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(ultra_sonic ${catkin_LIBRARIES} ${wiringPi_LIB})

add_executable(main_node src/main.cpp)
add_dependencies(main_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(main_node ${catkin_LIBRARIES})

install(PROGRAMS
	scripts/firebase_seed.js
	DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)






