#-
# $Copyright$
#

#*******************************************************************************
# Algorithm Library
#*******************************************************************************
set(TARGET_NAME algorithm)
set(TARGET_SRC
)
add_library(${TARGET_NAME} INTERFACE)
target_link_libraries(${TARGET_NAME}
)
target_include_directories(${TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#*******************************************************************************
# Testcases
#*******************************************************************************
if (UNITTEST AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/testcases)
    MESSAGE(STATUS "Building ${TARGET_NAME} Testcases")
    add_subdirectory(testcases)
endif()
