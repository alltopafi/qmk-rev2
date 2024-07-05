# alltopafi

![alltopafi](imgur.com image replace me!)

*A short description of the keyboard/project*

* Keyboard Maintainer: [alltopafi](https://github.com/alltopafi)
* Hardware Supported: *The PCBs, controllers supported*
* Hardware Availability: *Links to where you can find this hardware*

Make example for this keyboard (after setting up your build environment):

    make alltopafi:default

Flashing example for this keyboard:

    make alltopafi:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `RESET` if it is available


## Parts list

### Tools

1. Soldering Iron
2. Solder
3. Wire Strippers
4. [Switch dissasembly/lube kit*](https://www.amazon.com/gp/product/B097QXZ6PW/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
5. Hot Glue

### Parts

1. 68 - [Keyboard](https://www.amazon.com/dp/B07K55PPTV/ref=twister_B07K55Q1J7?_encoding=UTF8&th=1) Switches (Cherry MX Clear, lubbed)
2. 68 - [Diods](https://www.amazon.com/gp/product/B079KJ91JZ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
3. 1 - [PSP Joystick](https://www.amazon.com/dp/B01DNPQ1EM?psc=1&ref=ppx_yo2ov_dt_b_product_details) (not working in current firmware with IO expander)
4. 1 - [rotrary encoder]()https://www.amazon.com/dp/B08BFZMSCF?psc=1&ref=ppx_yo2ov_dt_b_product_details
5. 6 - [Wire](https://www.amazon.com/gp/product/B001IRVDV4/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1) (prefer multiple colors of solid core)
6. 1 - [Microcontoroller](https://www.boardsource.xyz/products/Elite-C_v4)
7. 1 - [IO Expander](https://www.amazon.com/dp/B08X6NDW3N?psc=1&ref=ppx_yo2ov_dt_b_product_details)
8. 2 - [OLED Screens](https://www.amazon.com/dp/B08ZY4YBHL?psc=1&ref=ppx_yo2ov_dt_b_product_details)
9. 2 - [USB-C breakout ports](https://www.amazon.com/dp/B09WQZW9GC?psc=1&ref=ppx_yo2ov_dt_b_product_details)
10. 4 - 3D printed case (top left, bottom left, top right, bottom left)
11. 1 - Keycap set (2 if doing split theme)
 
