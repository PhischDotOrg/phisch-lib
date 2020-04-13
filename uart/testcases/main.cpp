/*-
 * $Copyright$
-*/

#include <gtest/gtest.h>
#if defined(GMOCK_FOUND)
#include <gmock/gmock.h>
#endif /* defined(GMOCK_FOUND) */

int
main(int argc, char **argv) {
#if defined(GMOCK_FOUND)
    ::testing::InitGoogleMock(&argc, argv);
#else
    ::testing::InitGoogleTest(&argc, argv);
#endif /* defined(GMOCK_FOUND) */

    return RUN_ALL_TESTS();
}
