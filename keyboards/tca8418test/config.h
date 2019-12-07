/* Copyright 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */

#define F_SCL 100000UL  // SCL frequency

#define VENDOR_ID       0x1209
#define PRODUCT_ID      0xA1E5
#define DEVICE_VER      0x0008
#define MANUFACTURER    Technomancy
#define PRODUCT         Atreus
#define DESCRIPTION     QMK keyboard firmware for Atreus

/* key matrix size */
#define MATRIX_ROWS 4
#define MATRIX_COLS 12
#define MATRIX_COLS_PER_SIDE (MATRIX_COLS/2)

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* number of backlight levels */
//#define BACKLIGHT_LEVELS 3

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

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


/*
// Feather BLE Row & Column Pins
#define MATRIX_ROW_PINS { F7, F6, F5, F4 }
#define MATRIX_COL_PINS { F1, D6, B7, B6, B5, D7 }
*/

// ProMicro Row & Column Pins
#define MATRIX_ROW_PINS { F4, F5, F6, F7 }
#define MATRIX_COL_PINS { B5, B4, E6, D7, C6, D4 }

#define UNUSED_PINS

/* #define DIODE_DIRECTION COL2ROW */
#define DIODE_DIRECTION ROW2COL

// Mouse key movement acceleration
#define MOUSEKEY_DELAY 1
#define MOUSEKEY_INTERVAL 1
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_TIME_TO_MAX 255
#define MOUSEKEY_WHEEL_MAX_SPEED 7
#define MOUSEKEY_WHEEL_TIME_TO_MAX 255
