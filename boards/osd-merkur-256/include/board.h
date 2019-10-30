/*
 * Copyright (C) 2019 Harald Pichler
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_osd-merkur-256
 * @{
 *
 * @file
 * @brief       Board specific definitions for the OSD Merkurboard.
 *
 * @author      Harald Pichler <harald@the-develop.net>
 *              Ralf Schlatterbeck <rsc@runtux.com>
 *              Marcus Priesch <marcus@priesch.co.at>
 *
 */

#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   Baudrate for STDIO terminal
 *
 * The standard configuration for STDIO in spu/atmega_comman/periph/uart.c
 * is to use double speed.
 *
 * For 16MHz F_CPU following Baudrate have good error rates
 *
 *  38400
 *
 * Matches this with BAUD in Board/Makefile.include
 *
 * @{
 */
#ifndef STDIO_UART_BAUDRATE
#define STDIO_UART_BAUDRATE (38400U)       /**< Sets Baudrate for e.g. Shell */
#endif
/** @} */

/**
 * @name   LED pin definitions and handlers
 * @{
 */
#define LED_PORT            PORTE
#define LED_PORT_DDR        DDRE

#define LED0_MASK           (1 << DDE5)

#define LED0_ON             (LED_PORT &= ~LED0_MASK)
#define LED0_OFF            (LED_PORT |=  LED0_MASK)
#define LED0_TOGGLE         (LED_PORT ^=  LED0_MASK)
/** @} */

/**
 * @name   White LED light is used to signal ERROR.
 * @{
 */
#define LED_PANIC           LED0_ON
/** @} */

/**
 * @name xtimer configuration values
 * @{
 */
#define XTIMER_WIDTH        (16)
#define XTIMER_HZ           (250000UL)
/** @} */

/**
 * @name Indicate Watchdog cleared in bootloader an
 *
 * AVR CPUs need to reset the Watchdog as fast as possible.
 * This flag indicates that the watchdog is reseted in the bootloader
 * and that the MCUSR value is stored in register 2 (r2)
 * @{
 */
#define BOOTLOADER_CLEARS_WATCHDOG_AND_PASSES_MCUSR 1
/** @} */

/**
 * @brief Initialize board specific hardware, including clock, LEDs and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
