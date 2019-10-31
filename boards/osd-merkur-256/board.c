/*
 * Copyright (C) 2019 Harald Pichler <harald@the-develop.net>
 *
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_osd-merkurboard-256
 * @{
 *
 * @file
 * @brief       Board specific LED initialization
 *
 * @author      Harald Pichler <harald@the-develop.net>
 *
 *
 * @}
 */

#include "board.h"
#include "cpu.h"
#include <stdint.h>
#include "net/eui64.h"

extern uint8_t bootloader_get_mac (uint8_t);

void led_init(void)
{
    /* initialize the board LED */
    LED_PORT_DDR |= LED0_MASK;
    LED0_ON;
}

void board_eui64_get (eui64_t *address)
{
  uint64_t buf = 0;
  for (int x = 0; x < 8; x++) {
    buf <<= 8;
    buf |= bootloader_get_mac (7-x);
  }
  address->uint64.u64 = htonll (buf);
}