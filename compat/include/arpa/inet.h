/*-
 * $Copyright$
-*/

#ifndef _ARPA_INET_H_92da3057_ff2b_4cfa_8d4b_537bc8f4af86
#define _ARPA_INET_H_92da3057_ff2b_4cfa_8d4b_537bc8f4af86

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
inline uint32_t htonl(uint32_t hostlong) { return hostlong; }
inline uint16_t htons(uint16_t hostshort) { return hostshort; }
inline uint32_t ntohl(uint32_t netlong) { return netlong; }
inline uint16_t ntohs(uint16_t netshort) { return netshort; }
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
inline uint32_t htonl(uint32_t hostlong) {
    return (
            ((hostlong << 24) & 0xFF000000)
          | ((hostlong <<  8) & 0x00FF0000)
          | ((hostlong >>  8) & 0x0000FF00)
          | ((hostlong >> 24) & 0x000000FF)
    );
}

inline uint16_t htons(uint16_t hostshort) {
    return (
            ((hostshort << 8) & 0xFF00)
          | ((hostshort >> 8) & 0x00FF)
    );
}

inline uint32_t ntohl(uint32_t netlong) {
    return htonl(netlong);
}

inline uint16_t ntohs(uint16_t netshort) {
    return htons(netshort);
}

#else
#error Unknown Byte order
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */

#endif /* _ARPA_INET_H_92da3057_ff2b_4cfa_8d4b_537bc8f4af86 */
