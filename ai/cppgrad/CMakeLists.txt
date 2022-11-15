cmake_minimum_required(VERSION 3.10)
project(cppgrad VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


configure_file(config.h.in config.h)

add_executable(cppgrad cppgrad.cpp)

target_include_directories(cppgrad PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )


install (TARGETS
  cppgrad
  DESTINATION lib/${PROJECT_NAME}
)
