#-
# $Copyright$
#
cmake_minimum_required(VERSION 2.8)
project(phisch)

set(TARGET_NAME "compatc")
set(TARGET_SRC
    strlen.c
)
add_library(${TARGET_NAME} ${TARGET_SRC})
target_include_directories(${TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# target_compile_options(${TARGET_NAME} PRIVATE
#     -Wno-error=nonnull-compare
# )
