#-
# $Copyright$
#
add_definitions("-DUNITTEST")

#*******************************************************************************
# Testcases
#*******************************************************************************
set(TC_SOURCES
    test_UartDevice.cpp
)

#*******************************************************************************
# Test Executor
#*******************************************************************************
set(TARGET_EXE test_libs_uart)
add_executable(${TARGET_EXE} main.cpp ${TC_SOURCES})
target_link_libraries(${TARGET_EXE}
    gmock
    uart
)

#*******************************************************************************
# Configure CTest and the CDash Drop Location
#*******************************************************************************
add_test(NAME libuart COMMAND ${TARGET_EXE})

