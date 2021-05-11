#-
# $Copyright$
#

#*******************************************************************************
# Testcases Requirements
#*******************************************************************************
get_property(GMOCK_FOUND GLOBAL PROPERTY GMOCK_FOUND)
if(NOT GMOCK_FOUND)
    message(FATAL_ERROR "The libspi Testcases cannot be build without GMock!")
endif(NOT GMOCK_FOUND)

get_property(GMOCK_ROOT GLOBAL PROPERTY GMOCK_ROOT)
include_directories("${GMOCK_ROOT}/include")

get_property(GTEST_ROOT GLOBAL PROPERTY GTEST_ROOT)
include_directories("${GTEST_ROOT}/include")

get_property(SPI_ROOT GLOBAL PROPERTY SPI_ROOT)
include_directories("${SPI_ROOT}")

get_property(GPIO_ROOT GLOBAL PROPERTY GPIO_ROOT)
include_directories("${GPIO_ROOT}")

#*******************************************************************************
# Testcases
#*******************************************************************************
set(TC_SOURCES
    test_SpiDevice.cpp
    test_SpiAccessViaGpio.cpp
    test_SpiAccessViaI2c.cpp
    # test_SpiAccessViaSTM32F4.cpp
    test_SpiTransfer.cpp
)

#*******************************************************************************
# Test Executor
#*******************************************************************************
set(TARGET_EXE test_libs_spi)
add_executable(${TARGET_EXE} main.cpp ${TC_SOURCES})
target_link_libraries(${TARGET_EXE}
    gmock
)

#*******************************************************************************
# Configure CTest and the CDash Drop Location
#*******************************************************************************
add_test(NAME libspi COMMAND ${TARGET_EXE})

