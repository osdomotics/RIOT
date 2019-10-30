/*
 * Copyright (C) 2019 Harald Pichler
 *
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
 * @brief       Peripheral MCU configuration for the OSD Merkurboard
 *
 * @author      Harald Pichler <harald@the-develop.net>
 *              Ralf Schlatterbeck <rsc@runtux.com>
 *              Marcus Priesch <marcus@priesch.co.at> */

#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name   Clock configuration
 * @{
 */
#define CLOCK_CORECLOCK     (16000000UL)
/** @} */

#ifdef __cplusplus
}
#endif

#include "periph_conf_atmega_common.h"

#endif /* PERIPH_CONF_H */
/** @} */
