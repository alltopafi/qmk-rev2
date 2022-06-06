#include QMK_KEYBOARD_H
#include "alltopafi.h"
#include "i2c_master.h"
#include "mcp2301x.h"


void matrix_init_kb(void) {
    // put your keyboard start-up code here
    // runs once when the firmware starts up
    i2c_init();

    matrix_init_user();
}

void matrix_scan_kb(void) {
    // put your looping keyboard code here
    // runs every cycle (a lot)
    if (mcp2301x_reset_required()) {
        mcp2301x_init(MCP2301X_INPUT, MCP2301X_PULLUP, MCP2301X_ENABLED);
        // mcp2301x_encoder_init();
    }


    matrix_scan_user();
    // mcp2301x_encoder_read();
}
