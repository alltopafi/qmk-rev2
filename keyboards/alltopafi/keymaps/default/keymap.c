#include QMK_KEYBOARD_H
// #include "print.h"

#define KC_MC_CPY LGUI(KC_C)
#define KC_MC_PST LGUI(KC_V)

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _SECONDARY
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT( //TODO how do we allow the left side to work without the right side???
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     KC_GRAVE  ,KC_1    ,KC_2    ,KC_3    ,KC_4    ,KC_5    ,                                             KC_6    ,KC_7    ,KC_8    ,KC_9    ,KC_0    ,KC_MINS,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_TAB  ,KC_Q    ,KC_W    ,KC_E    ,KC_R    ,KC_T    ,KC_MC_CPY,                        KC_MC_PST, KC_Y    ,KC_U    ,KC_I    ,KC_O    ,KC_P    ,KC_BSLS ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_ESC ,KC_A    ,KC_S    ,KC_D    ,KC_F    ,KC_G     ,KC_LBRC,                           KC_RBRC ,KC_H    ,KC_J    ,KC_K    ,KC_L    ,KC_SCLN ,KC_QUOT ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     KC_LSFT    ,KC_Z    ,KC_X    ,KC_C    ,KC_V    ,KC_B    ,KC_DEL ,KC_MUTE ,        KC_EQL  ,_______ ,KC_N    ,KC_M    ,KC_COMM ,KC_DOT  ,KC_SLSH ,KC_RSFT ,
  //├────────┼────────┼──────56──┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     TG(1)   ,KC_MS_BTN1,KC_LCTL,KC_LALT     ,KC_LGUI     ,KC_SPC   ,KC_BSPC,        KC_ENT  ,MO(1)       ,KC_RGUI      ,KC_LEFT ,KC_DOWN ,KC_UP   ,KC_RGHT
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  ),
  [_SECONDARY] = LAYOUT( //TODO how do we allow the left side to work without the right side???
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                                           ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______ ,KC_F1   ,KC_F2   ,KC_F3   ,KC_F4   ,KC_F5   ,                                            KC_F6   ,KC_F7   ,KC_F8   ,KC_F9   ,KC_F10  ,KC_F11 ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐                         ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,_______ ,_______  ,_______ ,                         _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,KC_F12 ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┤                         ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ , _______,                          _______ ,KC_LEFT ,KC_DOWN ,KC_UP   ,KC_RGHT ,_______ ,_______ ,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┐       ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______   ,QK_BOOT ,        _______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,
  //├────────┼────────┼──────56──┼────────┼────┬───┴────┬───┼────────┼────────┤       ├────────┼────────┼───┬────┴───┬────┼────────┼────────┼────────┼────────┤
     TG(1) ,KC_MS_ACCEL0,KC_MS_ACCEL1,KC_MS_ACCEL2,_______,KC_MS_BTN1,KC_MS_BTN2,    _______ ,_______     ,_______      ,_______ ,_______ ,_______ ,_______
  //└────────┴────────┴────────┴────────┘    └────────┘   └────────┴────────┘       └────────┴────────┘   └────────┘    └────────┴────────┴────────┴────────┘
  )
};

#ifdef OLED_ENABLE
#include <stdio.h>
//#include "oled_display.h"
#include "oled_driver.h"

uint8_t oledState = 0;

static void render_logo(void) {
    print("show logo");
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };
    oled_write_P(qmk_logo, false);
}

bool oled_task_user(void) {
    if (oledState == 0) {
        // Clear OLED
        oled_clear();
        ++oledState;
        return false;
    } else if (oledState == 1) {
        // Display Logo
        render_logo();
        if(timer_read() < 5000) {
            return false;
        } else {
            oled_clear();
            ++oledState;
            return false;
        }
    }

    // for Keyboard Scan rate Up hack
    //   3 =  428 scan/sec
    //   4 =  808 scan/sec
    //   5 = 1141 scan/sec
    //   8 = 1970 scan/sec
    //  10 = 2400 scan/sec
    //  30 = 4565 scan/sec
    //  50 = 5346 scan/sec
    // 100 = 6120 scan/sec
    // 200 = 6575 scan/sec
    // if (oledState < 10) {
    //     ++oledState;
    //     return false;
    // }

    // oledState = 3;

    // Host Keyboard Layer Status
    oled_write_P(PSTR("To the Batcave!!!\n"), false);
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _SECONDARY:
            oled_write_P(PSTR("Secondary\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    // led_t led_state = host_keyboard_led_state();
    // oled_write_P(led_state.num_lock ? PSTR("[NUM]") : PSTR("[   ]"), led_state.num_lock);
    // oled_write_P(led_state.caps_lock ? PSTR("[CAP]") : PSTR("[   ]"), led_state.caps_lock);
    // oled_write_P(led_state.scroll_lock ? PSTR("[SCR]") : PSTR("[   ]"), led_state.scroll_lock);
    // oled_write_P(PSTR("\n"), false);

#ifdef WPM_ENABLE
    uint8_t n = get_current_wpm();
    char    wpm_str[4];
    wpm_str[3] = '\0';
    wpm_str[2] = '0' + n % 10;
    wpm_str[1] = '0' + (n /= 10) % 10;
    wpm_str[0] = '0' + n / 10;
    oled_write_P(PSTR("\nWPM: "), false);
    oled_write(wpm_str, false);
#endif

#ifdef DEBUG_MATRIX_SCAN_RATE
    char str[24] = "";
    sprintf(str, "Scan rate:%5ld/sec", get_matrix_scan_rate());
    oled_write(str, false);
#endif

    return false;
}
#endif

bool encoder_update_user(uint8_t index, bool clockwise) {
    if(clockwise) {
        if(get_mods() & MOD_MASK_GUI) {
            tap_code(KC_TAB);
        } else if(IS_LAYER_ON(0)) {
            tap_code_delay(KC_VOLU, 10);
        } else if(IS_LAYER_ON(1)) {
            tap_code_delay(KC_WH_U, 10);
        }
    } else {
        if(get_mods() & MOD_MASK_GUI) {
            register_mods(MOD_BIT(KC_LSFT));
            tap_code(KC_TAB);
            unregister_mods(MOD_BIT(KC_LSFT));
        } else if(IS_LAYER_ON(0)) {
            tap_code_delay(KC_VOLD, 10);
        } else if(IS_LAYER_ON(1)) {
            tap_code_delay(KC_WH_D, 10);
        }
    }

    return false;
}

void keyboard_post_init_user(void) {
//   // Customise these values to desired behaviour
  debug_enable=true;
//   debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
}


