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
    class UsbHwInEndpointMock : public ::usb::UsbHwInEndpoint {
    public:
        MOCK_METHOD(void, enable, (), (const));
        MOCK_METHOD(void, disable, (), (const));

        MOCK_METHOD(void, ack, (const size_t p_length), (const));
        MOCK_METHOD(void, stall, (), (const));
        MOCK_METHOD(void, nack, (), (const));

        MOCK_METHOD(void, registerEndpointCallback, (::usb::UsbInEndpoint &p_endpointCallout));
        MOCK_METHOD(void, unregisterEndpointCallback, ());

        MOCK_METHOD(void, setData, (unsigned p_dtog), (const));
        MOCK_METHOD(bool, getData, (), (const));
    };

    class UsbCtrlInEndpoint : public ::usb::UsbCtrlInEndpoint {
    public:
        UsbCtrlInEndpoint(::usb::UsbHwInEndpoint &p_usbHwInEndpointMock)
          : ::usb::UsbCtrlInEndpoint(p_usbHwInEndpointMock) {

        }
    };

protected:
    UsbHwInEndpointMock m_usbHwInEndpoint;
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
    UsbCtrlInEndpointTest() : m_ctrlInEndpoint(m_usbHwInEndpoint) {

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
