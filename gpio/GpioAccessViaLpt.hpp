/*-
 * $Copyright$
-*/

#ifndef _GPIO_ACCESS_VIA_LPT_HPP_2ee124ca_ba90_4e5c_bd6a_b8174edceaf5
#define _GPIO_ACCESS_VIA_LPT_HPP_2ee124ca_ba90_4e5c_bd6a_b8174edceaf5

#include <sys/types.h>
#include <stdint.h>

namespace gpio {

template <typename LptT>
class AccessViaLptT {
private:
    LptT * const m_lpt;
public:
    static const size_t m_width = 4;

    AccessViaLptT(LptT * const p_lpt);
    ~AccessViaLptT(void);

    int write(uint8_t p_value, uint8_t p_output, uint8_t p_mask) const;
    int read(uint8_t &p_vector) const;
};

}; /* namespace gpio */

#endif /* !_GPIO_ACCESS_VIA_LPT_HPP_2ee124ca_ba90_4e5c_bd6a_b8174edceaf5 */
