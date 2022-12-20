cmake_minimum_required(VERSION 3.10)
project(cpp_practice)

# Default to C++20
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(future future.cpp)

add_executable(async async.cpp)

add_executable(jthread jthread.cpp)

add_executable(circularq circularq.cpp)

add_executable(move move.cpp)

add_executable(coin coin.cpp)

add_executable(alien alien.cpp)

add_executable(phone phone.cpp)

add_executable(potpourri potpourri.cpp)

add_executable(interval interval.cpp)

add_executable(matrix01 matrix01.cpp)

add_executable(mine mine.cpp)

add_executable(tree tree.cpp)

add_executable(set set.cpp)

install (TARGETS
  future
  async
  jthread
  circularq
  move
  coin
  alien
  phone
  interval
  matrix01
  mine
  tree
  set
  potpourri
  DESTINATION lib/${PROJECT_NAME}
)
