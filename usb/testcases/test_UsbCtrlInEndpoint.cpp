/*-
 * $Copyright$
-*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <usb/UsbInEndpoint.hpp>
#include <usb/UsbTypes.hpp>

#if defined(__cplusplus)
extern "C" {
#endif  /* defined(__cplusplus) */

#if defined(__cplusplus)
}; /* extern "C" */
#endif /* defined(__cplusplus) */


using ::testing::_;

/******************************************************************************/
namespace usb {
/******************************************************************************/

/******************************************************************************/
class UsbCtrlInEndpointTest : public ::testing::Test {
private:
    class UsbCtrlInEndpoint : public ::usb::UsbCtrlInEndpoint {
    public:
        MOCK_METHOD(void, write, (const uint8_t * const p_data, const size_t p_length), (const, override));
    };

protected:
    UsbCtrlInEndpoint   m_ctrlInEndpoint;

    static constexpr auto m_shortStr = ::usb::UsbStringDescriptor(u"PhiSch.org");
    static constexpr auto m_multiPacketStr = ::usb::UsbStringDescriptor(
        u"A123456789abcdef" \
         "B123456789abcdef" \
         "C123456789abcdef" \
         "D123456789abcdef" \
         "E123456789abcdef" \
         "F123456789abcdef" \
         "G123456789abcdef" \
         "H123456789abcd"
    );

public:
    UsbCtrlInEndpointTest() {

    }

    void SetUp() override {
    }

    void TearDown() override {

    }
};
/******************************************************************************/

/******************************************************************************/
TEST_F(UsbCtrlInEndpointTest, CreateAndDelete) {
    /* Intentionally left blank */
}
/******************************************************************************/

/******************************************************************************/
} /* namespace usb */
/******************************************************************************/
