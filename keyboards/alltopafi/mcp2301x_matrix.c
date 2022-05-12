/*
 * Copyright 2020 Richard Titmuss (richard.titmuss@gmail.com)
 * Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar
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

#include QMK_KEYBOARD_H
#include "mcp2301x.h"

// TODO support ROW2COL

#define PRIMARY_COLS (sizeof(col_pins) / sizeof(pin_t))
#define SECONDARY_COLS (sizeof(secondary_col_pins) / sizeof(mcp2301x_pin_t))

static const pin_t          row_pins[]                 = MATRIX_ROW_PINS;
static const pin_t          col_pins[]                 = MATRIX_COL_PINS;
static const mcp2301x_pin_t secondary_row_pins[]       = MCP2301X_ROW_PINS;
static const mcp2301x_pin_t secondary_col_pins[]       = MCP2301X_COL_PINS;

static void select_row(uint8_t row) {
    setPinOutput(row_pins[row]);
    writePinLow(row_pins[row]);
}

static void unselect_row(uint8_t row) { setPinInputHigh(row_pins[row]); }

static void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        setPinInputHigh(row_pins[x]);
    }
}

static void select_secondary_row(uint8_t row) { mcp2301x_writePins(~secondary_row_pins[row]); }

static void init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < PRIMARY_COLS; x++) {
        setPinInputHigh(col_pins[x]);
    }
}

static matrix_row_t read_cols(void) {
    matrix_row_t state = 0;

    // For each col...
    for (uint8_t col_index = 0; col_index < PRIMARY_COLS; col_index++) {
        // Select the col pin to read (active low)
        uint8_t pin_state = readPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        state |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
    }

    return state;
}

static matrix_row_t read_secondary_cols(void) {
    matrix_row_t state = 0;

    mcp2301x_pin_t pins;
    if (mcp2301x_readPins(&pins)) {
        return 0;
    }

    for (uint8_t col_index = 0; col_index < SECONDARY_COLS; col_index++) {
        uint16_t pin_state = pins & (secondary_col_pins[col_index]);
        state |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
    }

    return state;
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    matrix_row_t last_row_value = current_matrix[current_row];

    select_row(current_row);
    select_secondary_row(current_row);

    current_matrix[current_row] = read_cols() | (read_secondary_cols() << PRIMARY_COLS);

    unselect_row(current_row);

    return (last_row_value != current_matrix[current_row]);
}

void matrix_init_custom(void) { init_pins(); }

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        changed |= read_cols_on_row(current_matrix, current_row);
    }

    return changed;
}

#ifdef POINTING_DEVICE_ENABLE
#include "analog.h"
#include "pointing_device.h"
#include "mcp2301x.h"

// Set Parameters
uint16_t minAxisValue = 200;
uint16_t maxAxisValue = 820;

uint8_t maxCursorSpeed = 2;
uint8_t precisionSpeed = 1;
uint8_t speedRegulator = 24;  // Lower Values Create Faster Movement

int8_t xPolarity = 1;
int8_t yPolarity = -1;

uint8_t cursorTimeout = 10;

int16_t xOrigin, yOrigin;

uint16_t lastCursor = 0;

int16_t axisCoordinate(uint8_t pin, uint16_t origin) {
    int8_t  direction;
    int16_t distanceFromOrigin;
    int16_t range;

    int16_t position = analogReadPin(pin);

    if (origin == position) {
        return 0;
    } else if (origin > position) {
        distanceFromOrigin = origin - position;
        range              = origin - minAxisValue;
        direction          = -1;
    } else {
        distanceFromOrigin = position - origin;
        range              = maxAxisValue - origin;
        direction          = 1;
    }

    float   percent    = (float)distanceFromOrigin / range;
    int16_t coordinate = (int16_t)(percent * 100);
    if (coordinate < 0) {
        return 0;
    } else if (coordinate > 100) {
        return 100 * direction;
    } else {
        return coordinate * direction;
    }
}

int8_t axisToMouseComponent(uint8_t pin, int16_t origin, uint8_t maxSpeed, int8_t polarity) {
    int coordinate = axisCoordinate(pin, origin);
    if (coordinate != 0) {
        float percent = (float)coordinate / 100;
        if (get_mods() & MOD_BIT(KC_LSFT)) {
            return percent * precisionSpeed * polarity * (abs(coordinate) / speedRegulator);
        } else {
            return percent * maxCursorSpeed * polarity * (abs(coordinate) / speedRegulator);
        }
    } else {
        return 0;
    }
}

void pointing_device_init(void) {
    // init pin? Is needed?
    // setPinInputHigh(E6);
    // Account for drift
    xOrigin = analogReadPin(B5);
    yOrigin = analogReadPin(B6);
}

void pointing_device_task(void) {
    report_mouse_t report = pointing_device_get_report();

    // todo read as one vector
    if (timer_elapsed(lastCursor) > cursorTimeout) {
        lastCursor = timer_read();
        report.x   = axisToMouseComponent(B5, xOrigin, maxCursorSpeed, xPolarity);
        report.y   = axisToMouseComponent(B6, yOrigin, maxCursorSpeed, yPolarity);
    }

    //
/*
    if (!readPin(E6)) {
        report.buttons |= MOUSE_BTN1;
    } else {
        report.buttons &= ~MOUSE_BTN1;
    }
*/

    pointing_device_set_report(report);
    pointing_device_send();
}
#endif
