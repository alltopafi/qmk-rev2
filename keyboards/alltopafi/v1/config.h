// Copyright 2021 Y.Sakamoto (@FREEWING-JP)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x7E57
#define DEVICE_VER   0x0001
#define MANUFACTURER alltopafi
#define PRODUCT      alltopafi-keyboard

/* key matrix size */
#define MATRIX_ROWS 10
#define MATRIX_COLS 14

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
#define MATRIX_COLS_PER_SIDE (MATRIX_COLS / 2)

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 */

//MCU B1, D7
//IO B0, A7
#define MATRIX_ROW_PINS_MCU \
    { F4, F5, F6, F7, B1 }
#define MATRIX_COL_PINS_MCU \
    { D3, D2, D4, C6, D7, E6, B4 }
#define UNUSED_PINS_MCU \
    {  }
 #define MATRIX_ROW_PINS_MCP \
    { B0, B1, B2, B3, B4 }
#define MATRIX_COL_PINS_MCP \
    { A0, A1, A2, A3, A4, A5, A6 }
#define UNUSED_PINS_MCP \
     {  }

#define ENCODERS_PAD_A { B6 }
#define ENCODERS_PAD_B { B2 }




// COL2ROW = Set row, Read cols
#define DIODE_DIRECTION COL2ROW

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT

/* disable action features */
// #define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT

/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define DYNAMIC_KEYMAP_LAYER_COUNT 2

/* Bootmagic Lite key configuration */
//#define BOOTMAGIC_LITE_ROW 0
//#define BOOTMAGIC_LITE_COLUMN 0

/*
 * Display OLED SSD1306 128x32
 */
#ifdef OLED_ENABLE
  #define OLED_DISPLAY_ADDRESS 0x3C
  #define OLED_BRIGHTNESS 255
  #define OLED_IC OLED_IC_SSD1306
  #define OLED_DISPLAY_WIDTH 128
  #define OLED_DISPLAY_HEIGHT 32
  #define OLED_TIMEOUT 31000
#endif

// #define DEBUG_MATRIX_SCAN_RATE
