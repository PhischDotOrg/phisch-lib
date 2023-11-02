#-
# $Copyright$
#
# cmake_minimum_required(VERSION 3.12)
set(DUT_NAME algorithm)
# project(test-${DUT_NAME})

enable_testing()

add_compile_options("-Wall")
add_compile_options("-Werror")

set(TARGET_NAME ${DUT_NAME}_tests)
set(TARGET_SRC
    testFifoCopy.cpp
)
add_executable(${TARGET_NAME} ${TARGET_SRC})
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS OFF
)
target_include_directories(${TARGET_NAME} PRIVATE
)
target_compile_definitions(${TARGET_NAME} PRIVATE
)
target_link_libraries(${TARGET_NAME}
    ${DUT_NAME}
    gtest
    gmock
    gmock_main
)

add_test(NAME ${DUT_NAME} COMMAND ${TARGET_NAME})
