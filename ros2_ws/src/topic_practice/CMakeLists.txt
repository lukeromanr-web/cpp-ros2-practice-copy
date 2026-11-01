cmake_minimum_required(VERSION 3.8)
project(topic_practice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(stage05_publisher src/stage05_publisher.cpp)
ament_target_dependencies(stage05_publisher rclcpp std_msgs)

add_executable(stage05_subscriber src/stage05_subscriber.cpp)
ament_target_dependencies(stage05_subscriber rclcpp std_msgs)

install(TARGETS stage05_publisher stage05_subscriber
  DESTINATION lib/${PROJECT_NAME})

ament_package()
