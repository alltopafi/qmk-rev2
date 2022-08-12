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
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no            # USB Nkey Rollover
AUDIO_ENABLE = no          # Audio output


POINTING_DEVICE_ENABLE = no # Generic Pointer
SRC += analog.c
POINTING_DEVICE_DRIVER = custom

OLED_ENABLE = yes           # Display Feature
OLED_DRIVER = SSD1306       # OLED SSD1306 128x32
WPM_ENABLE = yes

DYNAMIC_MACRO_ENABLE = no  # Dynamic Macro
VIA_ENABLE = no            # VIA Keymap function

# LINK_TIME_OPTIMIZATION_ENABLE = yes
LTO_ENABLE = yes # Config Option Link Time Optimization

ENCODER_ENABLE = yes

QMK_SETTINGS = yes
TAP_DANCE_ENABLE = no
COMBO_ENABLE = no
KEY_OVERRIDE_ENABLE = no


CUSTOM_MATRIX = lite
USE_RIGHT_SIDE = yes

SRC += mcp2301x.c \
    mcp2301x_matrix.c \
	pointer.c

QUANTUM_LIB_SRC += i2c_master.c
