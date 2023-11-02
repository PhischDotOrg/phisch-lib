
/*-
 * $Copyright$
 */

#include <gtest/gtest.h>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <vector>

#include "fifocopy.hpp"

/******************************************************************************/
template<
    size_t Width,
    typename ElementType
>
class FifoCopyTest : public ::testing::TestWithParam<size_t> {
protected:
    static constexpr size_t m_bufferSz = 64u;

    const std::array<uint8_t, m_bufferSz> m_sourceData {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    };
    static_assert(m_bufferSz == sizeof(m_sourceData));

    std::array<uint8_t, m_bufferSz> m_targetData;
    static_assert(sizeof(m_sourceData) == sizeof(m_targetData));

    using ArrayT = std::array<ElementType, (m_bufferSz / Width)>;

    ArrayT m_packet;

    std::pair<size_t, size_t>
    copyData(void) {
        size_t byteCount = GetParam();

        size_t to = stm32::copy_to_fifo<Width, stm32::copy_to_fifo::WriteCurrentPos>(m_sourceData, m_packet, byteCount);
        EXPECT_EQ(to, byteCount);

        size_t from = stm32::copy_from_fifo<Width, stm32::copy_from_fifo::ReadCurrentPos>(m_packet, m_targetData, byteCount);
        EXPECT_EQ(from, byteCount);

        EXPECT_TRUE(std::equal(m_targetData.cbegin(), m_targetData.cbegin() + byteCount, m_sourceData.begin()));

        return std::make_pair(to, from);
    }

    std::pair<size_t, size_t>
    copyPacketLargerThanBuffer(void) {
        std::array<uint8_t, m_bufferSz - 1> targetBuffer;
        std::array<ElementType, (m_bufferSz / Width)> packet;

        size_t byteCount = GetParam();

        auto to = stm32::copy_to_fifo<Width, stm32::copy_to_fifo::WriteCurrentPos>(m_sourceData, packet, byteCount);
        EXPECT_EQ(to, std::min(byteCount, std::min(sizeof(m_sourceData), sizeof(packet))));

        auto from = stm32::copy_from_fifo<Width, stm32::copy_from_fifo::ReadCurrentPos>(packet, targetBuffer, byteCount);
        EXPECT_EQ(from, std::min(byteCount, std::min(sizeof(targetBuffer), sizeof(packet))));

        EXPECT_TRUE(std::equal(targetBuffer.cbegin(), std::min(targetBuffer.cbegin() + byteCount, targetBuffer.cend()), m_sourceData.begin()));

        return std::make_pair(to, from);
    }

    std::pair<size_t, size_t>
    copyPacketSmallerThanBuffer(void) {
        std::array<uint8_t, m_bufferSz + 1> targetBuffer;
        std::array<ElementType, (m_bufferSz / Width)> packet;

        size_t byteCount = GetParam();

        auto to = stm32::copy_to_fifo<Width, stm32::copy_to_fifo::WriteCurrentPos>(m_sourceData, packet, byteCount);
        EXPECT_EQ(to, std::min(byteCount, std::min(sizeof(m_sourceData), sizeof(packet))));

        auto from = stm32::copy_from_fifo<Width, stm32::copy_from_fifo::ReadCurrentPos>(packet, targetBuffer, byteCount);
        EXPECT_EQ(from, std::min(byteCount, std::min(sizeof(targetBuffer), sizeof(packet))));

        EXPECT_TRUE(std::equal(m_sourceData.cbegin(), std::min(m_sourceData.cbegin() + byteCount, m_sourceData.cend()), targetBuffer.begin()));

        std::cout << "p_byteCount=" << byteCount
          << " to=" << to
          << " from=" << from
          << std::endl;

        return std::make_pair(to, from);
    }
};
/******************************************************************************/

/******************************************************************************/
namespace FifoCopyGeneric {
/******************************************************************************/
struct FifoCopyTestGeneric : public FifoCopyTest<1, uint8_t> {
};
/******************************************************************************/

TEST_F(FifoCopyTestGeneric, InNullPtr) {
    std::array<uint8_t, 1>  in, out;

    auto len = stm32::copy_from_fifo<1, stm32::copy_from_fifo::ReadCurrentPos, decltype(in)::const_iterator>(nullptr, nullptr, out.begin(), out.end());
    EXPECT_EQ(0, len);
}

TEST_F(FifoCopyTestGeneric, InBeginBeyondEnd) {
    std::array<uint8_t, 1>  in, out;

    auto len = stm32::copy_from_fifo<1, stm32::copy_from_fifo::ReadCurrentPos, decltype(in)::const_iterator>(in.end() + 1, in.end(), out.begin(), out.end());
    EXPECT_EQ(0, len);
}

TEST_F(FifoCopyTestGeneric, OutNullPtr) {
    std::array<uint8_t, 0>  in, out;

    auto len = stm32::copy_from_fifo<1, stm32::copy_from_fifo::ReadCurrentPos, decltype(in)::const_iterator, decltype(out)::iterator>(in.begin(), in.end(), nullptr, nullptr);
    EXPECT_EQ(0, len);
}

TEST_F(FifoCopyTestGeneric, OutBeyondEnd) {
    std::array<uint8_t, 1>  in, out;

    auto len = stm32::copy_from_fifo<1, stm32::copy_from_fifo::ReadCurrentPos, decltype(in)::const_iterator>(in.end(), in.end(), out.end() + 1, out.end());
    EXPECT_EQ(0, len);
}

/******************************************************************************/
} /* namespace FifoCopyGeneric */
/******************************************************************************/

/******************************************************************************/
namespace FifoCopyByte {
/******************************************************************************/
struct FifoCopyByteTest : public FifoCopyTest<1, uint8_t> {
};

TEST_P(FifoCopyByteTest, Copy) {
    copyData();
}

TEST_P(FifoCopyByteTest, CopyPacketLargerThanBuffer) {
    copyPacketLargerThanBuffer();
}

TEST_P(FifoCopyByteTest, CopyPacketSmallerThanBuffer) {
    copyPacketSmallerThanBuffer();
}

INSTANTIATE_TEST_SUITE_P(
    FifoCopyByteTests,
    FifoCopyByteTest,
    ::testing::Values(
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 63, 64
    )
);
/******************************************************************************/
} /* namespace FifoCopyByte */
/******************************************************************************/

/******************************************************************************/
namespace FifoCopyHalfWord {
/******************************************************************************/
struct FifoCopyHalfWordTest : public FifoCopyTest<2, uint16_t> {
};

TEST_P(FifoCopyHalfWordTest, Copy) {
    copyData();
}

TEST_P(FifoCopyHalfWordTest, CopyPacketLargerThanBuffer) {
    copyPacketLargerThanBuffer();
}

TEST_P(FifoCopyHalfWordTest, CopyPacketSmallerThanBuffer) {
    copyPacketSmallerThanBuffer();
}

INSTANTIATE_TEST_SUITE_P(
    FifoCopyHalfWordTests,
    FifoCopyHalfWordTest,
    ::testing::Values(
        1, 2, 3, 4, 5, 6, 7, 8, 9, 63, 64
    )
);

/******************************************************************************/
} /* namespace FifoCopyHalfWord */
/******************************************************************************/

/******************************************************************************/
namespace FifoCopyWord {
/******************************************************************************/
struct FifoCopyWordTest : public FifoCopyTest<4, uint32_t> {
};

TEST_P(FifoCopyWordTest, Copy) {
    copyData();
}

TEST_P(FifoCopyWordTest, CopyPacketLargerThanBuffer) {
    copyPacketLargerThanBuffer();
}

TEST_P(FifoCopyWordTest, CopyPacketSmallerThanBuffer) {
    copyPacketSmallerThanBuffer();
}

INSTANTIATE_TEST_SUITE_P(
    FifoCopyWordTests,
    FifoCopyWordTest,
    ::testing::Values(
        1, 2, 3, 4, 5, 6, 7, 8, 9, 63, 64
    )
);

/******************************************************************************/
} /* namespace FifoCopyWord */
/******************************************************************************/

/******************************************************************************/
namespace FifoCopyScattered {
/******************************************************************************/
struct ScatteredMemory {
    union {
        uint16_t    m_data16;
        uint8_t     m_data8[2];
    }           m_data;
    uint16_t    m_reserved;

    /*
     * This conversion operator is used when a std::array<ScatteredMemory, ...>::iterator
     * is dereferenced and used in bit-shift operation. Notably, this happens in the
     * stm32::copy() function.
     */
    operator int() const { return m_data.m_data16; }

    ScatteredMemory(void) = default;
    constexpr ScatteredMemory(uint16_t p_data) : m_data { .m_data16 = p_data }, m_reserved(0) { };
} __attribute__((packed));
static_assert(4 == sizeof(ScatteredMemory));
static_assert(2 == sizeof(ScatteredMemory::m_data));

struct FifoCopyScatteredTest : public FifoCopyTest<2, ScatteredMemory> {
};

TEST_P(FifoCopyScatteredTest, CopyFull) {
    copyData();
}

TEST_P(FifoCopyScatteredTest, CopyPacketLargerThanBuffer) {
    copyPacketLargerThanBuffer();
}

TEST_P(FifoCopyScatteredTest, CopyPacketSmallerThanBuffer) {
    copyPacketSmallerThanBuffer();
}

INSTANTIATE_TEST_SUITE_P(
    FifoCopyScatteredTests,
    FifoCopyScatteredTest,
    ::testing::Values(
        1, 2, 3, 4, 5, 6, 7, 8, 9, 63, 64
    )
);

/******************************************************************************/
} /* namespace FifoCopyScattered */
/******************************************************************************/

TEST(FifoCopy, STM32F4_InEndpoint) {
    static constexpr size_t m_bufferSz = 64u;

    const std::array<uint8_t, m_bufferSz> m_sourceData {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    };
    static_assert(m_bufferSz == sizeof(m_sourceData));

    using ArrayT = std::array<uint32_t, (m_bufferSz / sizeof(uint32_t))>;
    ArrayT m_fifoBuffer;

    const uint32_t * const m_fifoAddr = m_fifoBuffer.data();
    static_assert(sizeof(*m_fifoAddr) == sizeof(uint32_t));
    static_assert(sizeof(m_sourceData) <= sizeof(m_fifoBuffer));

    size_t len = stm32::copy_to_fifo<sizeof(*m_fifoAddr), stm32::copy_to_fifo::WriteCurrentPos>(m_sourceData.begin(), m_sourceData.end(), m_fifoBuffer.begin(), m_fifoBuffer.end());
    EXPECT_EQ(len, m_bufferSz);

    for (unsigned idx = 0; idx < m_fifoBuffer.size(); idx++) {
        const uint32_t val = * reinterpret_cast<const uint32_t *>(&m_sourceData[idx * sizeof(uint32_t)]);
        EXPECT_EQ(m_fifoBuffer[idx], val) << "Difference at idx=" << idx;
    }
}

TEST(FifoCopy, STM32F4_InEndpoint_PushToBegin) {
    static constexpr size_t m_bufferSz = 64u;

    const std::array<uint8_t, m_bufferSz> m_sourceData {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    };
    static_assert(m_bufferSz == sizeof(m_sourceData));

    using ArrayT = std::array<uint32_t, (m_bufferSz / sizeof(uint32_t))>;
    ArrayT m_fifoBuffer {};

    const uint32_t * const m_fifoAddr = m_fifoBuffer.data();
    static_assert(sizeof(*m_fifoAddr) == sizeof(uint32_t));
    static_assert(sizeof(m_sourceData) <= sizeof(m_fifoBuffer));

    size_t len = stm32::copy_to_fifo<sizeof(*m_fifoAddr), stm32::copy_to_fifo::PushToBegin>(m_sourceData.begin(), m_sourceData.end(), m_fifoBuffer.begin(), m_fifoBuffer.end());
    EXPECT_EQ(len, m_bufferSz);

    const uint32_t val = * reinterpret_cast<const uint32_t *>(&m_sourceData[(m_fifoBuffer.size() - 1) * sizeof(uint32_t)]);
    EXPECT_EQ(m_fifoBuffer[0], val);

    for (unsigned idx = 1; idx < m_fifoBuffer.size(); idx++) {
        EXPECT_EQ(0, m_fifoBuffer[idx]);
    }
}
