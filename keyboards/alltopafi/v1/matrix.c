/*
Copyright 2013 Oleg Kostyuk <cub.uanic@gmail.com>
          2020 Pierre Chevalier <pierrechevalier83@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * This code was heavily inspired by the ergodox_ez keymap, and modernized
 * to take advantage of the quantum.h microcontroller agnostics gpio control
 * abstractions and use the macros defined in config.h for the wiring as opposed
 * to repeating that information all over the place.
 */

#include QMK_KEYBOARD_H
#include "i2c_master.h"

extern i2c_status_t mcp23017_status;
#define I2C_TIMEOUT 1000

// For a better understanding of the i2c protocol, this is a good read:
// https://www.robot-electronics.co.uk/i2c-tutorial

// I2C address:
// See the datasheet, section 3.3.1 on addressing I2C devices and figure 3-6 for an
// illustration
// http://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf
// All address pins of the mcp23017 are connected to the ground on the ferris
// | 0  | 1  | 0  | 0  | A2 | A1 | A0 |
// | 0  | 1  | 0  | 0  | 0  | 0  | 0  |
#define I2C_ADDR 0b0100000
#define I2C_ADDR_WRITE ((I2C_ADDR << 1) | I2C_WRITE)
#define I2C_ADDR_READ ((I2C_ADDR << 1) | I2C_READ)

// Register addresses
// See https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library/blob/master/Adafruit_MCP23017.h
#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01
#define GPPUA 0x0C  // GPIO pull-up resistor register
#define GPPUB 0x0D
#define GPIOA 0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB 0x13
#define OLATA 0x14  // output latch register
#define OLATB 0x15

bool         i2c_initialized = 0;
i2c_status_t mcp23017_status = I2C_ADDR;

uint8_t init_mcp23017(void) {
    print("starting init\n");
    mcp23017_status = I2C_ADDR;

    // I2C subsystem
    if (i2c_initialized == 0) {
        i2c_init();  // on pins D(1,0)
        i2c_initialized = true;
        wait_ms(I2C_TIMEOUT);
    }

    // set pin direction
    // - unused  : input  : 1
    // - input   : input  : 1
    // - driving : output : 0
    // This means: we will read all the bits on GPIOA
    // This means: we will write to the pins 0-4 on GPIOB (in select_rows)
    uint8_t buf[]   = {IODIRA, 0b11111111, 0b11100000};
    mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
    if (!mcp23017_status) {
        // set pull-up
        // - unused  : on  : 1
        // - input   : on  : 1
        // - driving : off : 0
        // This means: we will read all the bits on GPIOA
        // This means: we will write to the pins 0-4 on GPIOB (in select_rows)
        uint8_t pullup_buf[] = {GPPUA, 0b11111111, 0b11100000};
        mcp23017_status      = i2c_transmit(I2C_ADDR_WRITE, pullup_buf, sizeof(pullup_buf), I2C_TIMEOUT);
    }
    return mcp23017_status;
}

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];  // debounced values

static matrix_row_t read_cols(uint8_t row);
static void         init_cols(void);
static void         unselect_rows(void);
static void         select_row(uint8_t row);

static uint8_t mcp23017_reset_loop;

void matrix_init_custom(void) {
    // initialize row and col
    print("calling matrix init custom\n");
    mcp23017_status = init_mcp23017();

    unselect_rows();
    init_cols();
    // print("post init cols call\n");

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        uprintf("init matrix for row %d\n", i);

    }
}

void matrix_power_up(void) {
    print("calling matrix power up\n");
    mcp23017_status = init_mcp23017();

    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
    }
}

// Reads and stores a row, returning
// whether a change occurred.
static inline bool store_matrix_row(matrix_row_t current_matrix[], uint8_t index) {
    // print("calling store_matrix_row\n");
    matrix_row_t temp = read_cols(index);
    if (current_matrix[index] != temp) {
        uprintf("temp value is: %d and current value is: %d\n", temp, current_matrix[index]);
        current_matrix[index] = temp;
        return true;
    }
    return false;
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    if (mcp23017_status) {  // if there was an error
        if (++mcp23017_reset_loop == 0) {
            // if (++mcp23017_reset_loop >= 1300) {
            // since mcp23017_reset_loop is 8 bit - we'll try to reset once in 255 matrix scans
            // this will be approx bit more frequent than once per second
            print("trying to reset mcp23017\n");
            mcp23017_status = init_mcp23017();
            if (mcp23017_status) {
                print("right side not responding\n");
            } else {
                print("right side attached\n");
            }
        }
    }

    bool changed = false;
    for (uint8_t i = 0; i < MATRIX_ROWS_PER_SIDE; i++) {
        // select rows from left and right hands
        uint8_t left_index  = i;
        uint8_t right_index = i + MATRIX_ROWS_PER_SIDE;
        // uprintf("index for left side select row %d\n", left_index);
        // uprintf("index for right side select row %d\n", right_index);
        select_row(left_index);
        select_row(right_index);

        // we don't need a 30us delay anymore, because selecting a
        // left-hand row requires more than 30us for i2c.

        changed |= store_matrix_row(current_matrix, left_index);
        changed |= store_matrix_row(current_matrix, right_index);
        // uprintf("has something changed: %b\n", changed);

        unselect_rows();
    }

    return changed;
}

static void init_cols(void) {
    // init on mcp23017
    // not needed, already done as part of init_mcp23017()
    // print("calling init_cols\n");

    // init on mcu
    pin_t matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
    for (int pin_index = 0; pin_index < MATRIX_COLS_PER_SIDE; pin_index++) {
        uprintf("calling init cols for pin index: %d\n", pin_index);
        pin_t pin = matrix_col_pins_mcu[pin_index];
        setPinInput(pin);
        writePinHigh(pin);
    }
}

static matrix_row_t read_cols(uint8_t row) {
    if (row < MATRIX_ROWS_PER_SIDE) {
        pin_t        matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
        matrix_row_t current_row_value                         = 0;
        // For each col...
        for (uint8_t col_index = 0; col_index < MATRIX_COLS_PER_SIDE; col_index++) {
            // Select the col pin to read (active low)
            uint8_t pin_state = readPin(matrix_col_pins_mcu[col_index]);

            // Populate the matrix row with the state of the col pin
            current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
        }
        return current_row_value;
    } else {
        if (mcp23017_status) {  // if there was an error
            return 0;
        } else {
            uint8_t buf[]   = {GPIOA};
            mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
            // We read all the pins on GPIOA.
            // The initial state was all ones and any depressed key at a given column for the currently selected row will have its bit flipped to zero.
            // The return value is a row as represented in the generic matrix code were the rightmost bits represent the lower columns and zeroes represent non-depressed keys while ones represent depressed keys.
            // Since the pins connected to eact columns are sequential, and counting from zero up (col 5 -> GPIOA0, col 6 -> GPIOA1 and so on), the only transformation needed is a bitwise not to swap all zeroes and ones.
            uint8_t data[] = {0};
            if (!mcp23017_status) {
                mcp23017_status = i2c_receive(I2C_ADDR_READ, data, sizeof(data), I2C_TIMEOUT);
                data[0]         = ~(data[0]);
            }
            return data[0];
        }
    }
}

static void unselect_rows(void) {
    // no need to unselect on mcp23017, because the select step sets all
    // the other row bits high, and it's not changing to a different
    // direction
    // print("calling unselect_rows\n");

    // unselect rows on microcontroller
    pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
    for (int pin_index = 0; pin_index < MATRIX_ROWS_PER_SIDE; pin_index++) {
        pin_t pin = matrix_row_pins_mcu[pin_index];
        setPinInput(pin);
        writePinLow(pin);
    }
}

static void select_row(uint8_t row) {
        // print("calling select_row\n");

    if (row < MATRIX_ROWS_PER_SIDE) {
        // select on atmega32u4
        pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
        pin_t pin                                       = matrix_row_pins_mcu[row];
        setPinOutput(pin);
        writePinLow(pin);
    } else {
        // select on mcp23017
        if (mcp23017_status) {  // if there was an error
                                // do nothing
            // print("error during select_row\n");
        } else {
            // Select the desired row by writing a byte for the entire GPIOB bus where only the bit representing the row we want to select is a zero (write instruction) and every other bit is a one.
            // Note that the row - MATRIX_ROWS_PER_SIDE reflects the fact that being on the right hand, the columns are numbered from MATRIX_ROWS_PER_SIDE to MATRIX_ROWS, but the pins we want to write to are indexed from zero up on the GPIOB bus.
            uint8_t buf[]   = {GPIOB, 0xFF & ~(1 << (row - MATRIX_ROWS_PER_SIDE))};
            mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);

            // uprintf("row: %d\nMatrix rows per side%d\nbuf: %d", row , MATRIX_ROWS_PER_SIDE, buf);
        }
    }
}


#ifdef POINTING_DEVICE_ENABLE
#include "analog.h"
#include "pointing_device.h"
#include "mcp23017.h"

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
    xOrigin = analogReadPin(GPA0);
    yOrigin = analogReadPin(GPA1);
}

void pointing_device_task(void) {
    report_mouse_t report = pointing_device_get_report();

    // todo read as one vector
    if (timer_elapsed(lastCursor) > cursorTimeout) {
        lastCursor = timer_read();
        report.x   = axisToMouseComponent(GPA0, xOrigin, maxCursorSpeed, xPolarity);
        report.y   = axisToMouseComponent(GPA1, yOrigin, maxCursorSpeed, yPolarity);
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
