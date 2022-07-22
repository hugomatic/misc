cmake_minimum_required(VERSION 3.5)
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

install (TARGETS
  future
  async
  jthread
  circularq
  move
  coin
  alien
  phone
  DESTINATION lib/${PROJECT_NAME}
)
