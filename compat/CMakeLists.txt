#-
# $Copyright$
#
set(TARGET_NAME "compatc")
set(TARGET_SRC
    strlen.c
)

if (UNITTEST)
    add_library(${TARGET_NAME} INTERFACE)
else()
    add_library(${TARGET_NAME} ${TARGET_SRC})
    # target_include_directories(${TARGET_NAME} INTERFACE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/include
    # )
endif()
