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

void led_init(void)
{
    /* initialize the board LED */
    LED_PORT_DDR |= LED0_MASK;
    LED0_ON;
}
