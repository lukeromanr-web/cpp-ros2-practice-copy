cmake_minimum_required(VERSION 3.20)
project(cpp_ros2_practice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Threads REQUIRED)

add_executable(stage01 lessons/ros2_pubsub/stage01_plain.cpp)
add_executable(stage02 lessons/ros2_pubsub/stage02_class.cpp)
add_executable(stage03 lessons/ros2_pubsub/stage03_template_ptr.cpp)
add_executable(stage04 lessons/ros2_pubsub/stage04_callback_timer.cpp)
target_link_libraries(stage04 PRIVATE Threads::Threads)

add_executable(vector_publisher_todo lessons/vector/publisher_todo.cpp)
add_executable(vector_publisher_solution lessons/vector/publisher_solution.cpp)
add_executable(monitor_todo lessons/vector/monitor_todo.cpp)
add_executable(monitor_solution lessons/vector/monitor_solution.cpp)

set(PRACTICE_TARGETS stage01 stage02 stage03 stage04
    vector_publisher_todo vector_publisher_solution monitor_todo monitor_solution)
foreach(target IN LISTS PRACTICE_TARGETS)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endforeach()
install(TARGETS ${PRACTICE_TARGETS} RUNTIME DESTINATION bin)

include(CTest)
if(BUILD_TESTING)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
  add_test(NAME teaching_stages COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/verify_cpp.py
    --bin $<TARGET_FILE_DIR:stage01> --mode teaching)
  add_test(NAME vector_reference COMMAND ${Python3_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/verify_cpp.py
    --bin $<TARGET_FILE_DIR:monitor_solution> --mode reference)
endif()
