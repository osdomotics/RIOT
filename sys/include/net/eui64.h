/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    net_eui64   IEEE EUI-64 identifier
 * @ingroup     net
 * @brief       Type definiton of the IEEE EUI-64 identifier
 * @see         <a href="http://standards.ieee.org/regauth/oui/tutorials/EUI64.html">
 *                  IEEE, "Guidelines for 64-bit Global Identifier (EUI-64)"
 *              </a>
 * @{
 *
 * @file
 * @brief   EUI-64 data type definition
 *
 * @author  Martine Lenders <mlenders@inf.fu-berlin.de>
 * @author  Oliver Hahm <oliver.hahm@inria.fr>
 */
#ifndef NET_EUI64_H
#define NET_EUI64_H

#include <stdint.h>
#include "byteorder.h"
#include "luid.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IEEE 802.15.4 address lengths
 * @{
 */
#define IEEE802154_SHORT_ADDRESS_LEN (2U) /**< short (16-bit) address */
#define IEEE802154_LONG_ADDRESS_LEN  (8U) /**< long address (EUI-64) */

/**
 * @brief Data type to represent an EUI-64.
 */
typedef union {
    network_uint64_t uint64;     /**< represented as 64 bit value */
    uint8_t uint8[8];            /**< split into 8 8-bit words. */
    network_uint16_t uint16[4];  /**< split into 4 16-bit words. */
} eui64_t;

inline void eui64_get (eui64_t *address)
{
#ifdef BOARD_PROVIDES_EUI64
  extern void board_eui64_get (eui64_t *address);
  board_eui64_get (address);
#else
  /* get an 8-byte unique ID to use as hardware address */
  luid_get(address->uint8, IEEE802154_LONG_ADDRESS_LEN);
  /* make sure we mark the address as non-multicast and not globally unique */
  address->uint8[0] &= ~(0x01);
  address->uint8[0] |=  (0x02);
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* NET_EUI64_H */
/** @} */
