# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = yes         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no            # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no          # Audio output

POINTING_DEVICE_ENABLE = no # Generic Pointer
SRC += analog.c
POINTING_DEVICE_DRIVER = custom
# POINTING_DEVICE_DRIVER = analog_joystick



OLED_ENABLE = no           # Display Feature
OLED_DRIVER = SSD1306       # OLED SSD1306 128x32
#ifeq ($(strip $(OLED_ENABLE)), yes)
#    OLED_DRIVER_ENABLE = yes
#endif

DYNAMIC_MACRO_ENABLE = yes  # Dynamic Macro

VIA_ENABLE = yes            # VIA Keymap function

# LINK_TIME_OPTIMIZATION_ENABLE = yes
LTO_ENABLE = yes # Config Option Link Time Optimization

CUSTOM_MATRIX = lite

SRC += matrix.c
QUANTUM_LIB_SRC += i2c_master.c

ENCODER_ENABLE = yes

QMK_SETTINGS = yes
TAP_DANCE_ENABLE = no
COMBO_ENABLE = no
KEY_OVERRIDE_ENABLE = no

