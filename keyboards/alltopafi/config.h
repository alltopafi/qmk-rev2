// Copyright 2022 alltopafi (@alltopafi)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
/* VID & PID from vusb project, see tmk_core/protocol/vusb/USB-IDs-for-free.txt"*/
#define VENDOR_ID 0x7274
#define PRODUCT_ID 0x0001
#define DEVICE_VER 0x0000
#define MANUFACTURER richard.titmuss
#define PRODUCT Torn

/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 14

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */
#define MATRIX_ROW_PINS \
    { F4, F5, F6, F7, B1 }
#define MATRIX_COL_PINS \
    { D3, D2, D4, C6, D7, E6, B4 }
#define UNUSED_PINS

#define MCP2301X_ROW_PINS \
    { GPA7, GPA6, GPA5, GPA4, GPA3 }
#define MCP2301X_COL_PINS \
    { GPB0, GPB1, GPB2, GPB3, GPB4, GPB5, GPB6 }

#define MCP2301X_INPUT   ( GPB3 | GPB2 | GPB1 | GPB0 | GPB4 | GPB5 )
#define MCP2301X_PULLUP  ( GPB3 | GPB2 | GPB1 | GPB0 | GPB4 | GPB5 )
#define MCP2301X_ENABLED ( GPB0 | GPB1 | GPB2 | GPA2 | GPA1)

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/* #define ENCODERS_PAD_A \
//     { B2 }
// #define ENCODERS_PAD_B \
//     { B1 }

// #define MCP2301X_ENCODERS_PAD_A { GPB5 }
// #define MCP2301X_ENCODERS_PAD_B { GPB4 }

#define USB_MAX_POWER_CONSUMPTION 100
*/

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

#define OLED_TIMEOUT 60000
