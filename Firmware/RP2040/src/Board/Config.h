#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#define PI_PICO             0
#define RP2040_ZERO         1
#define ADAFRUIT_FEATHER    2
#define PI_PICOW            3
#define ESP32_BLUEPAD32_I2C 4
#define ESP32_BLUERETRO_I2C 5
#define EXTERNAL_4CH_I2C    6
#define INTERNAL_4CH_I2C    7
#define RP2350_USB_A        8
#define RP2350_ZERO         9
#define RP2040_XIAO         10
#define RP2354              11
#define BOARDS_COUNT        12

#define SYSCLOCK_KHZ 240000

#ifndef MAX_GAMEPADS
    #define MAX_GAMEPADS 1
#endif

/** TinyUSB XInput host instances. The 360 wireless USB receiver exposes 4 controller interfaces;
 *  all 4 must be polled even when MAX_GAMEPADS is 1 so sync works on any RF slot. */
#if MAX_GAMEPADS < 4
    #define OGXM_TUH_XINPUT_INSTANCES 4
#else
    #define OGXM_TUH_XINPUT_INSTANCES MAX_GAMEPADS
#endif

/** Main loop delay (device/core0) in microseconds. 0 = low latency (default). 250+ = lower CPU. */
#ifndef MAIN_LOOP_DELAY_US
    #define MAIN_LOOP_DELAY_US 0
#endif

#if defined(CONFIG_OGXM_BOARD_PI_PICO) || defined(CONFIG_OGXM_BOARD_PI_PICO2)
    #define OGXM_BOARD          PI_PICO
    #define PIO_USB_DP_PIN      0 // DM = 1
    #define LED_INDICATOR_PIN   25

#elif defined(CONFIG_OGXM_BOARD_PI_PICOW) || defined(CONFIG_OGXM_BOARD_PI_PICO2W)
    #define OGXM_BOARD          PI_PICOW
    // PIO USB host for external port (e.g. Wii mode: controller on USB, BT for Wiimote)
    #define PIO_USB_DP_PIN      0  // D+ = GP0, D- = GP1
    #ifndef PIO_USB_SWAP_DP_DM
    #define PIO_USB_SWAP_DP_DM  0  // set 1 if controller powers but does not enumerate
    #endif

#elif defined(CONFIG_OGXM_BOARD_RP2040_ZERO)
    #define OGXM_BOARD          RP2040_ZERO
    #define RGB_PXL_PIN         16 
    #define PIO_USB_DP_PIN      10 // DM = 11
    #define LED_INDICATOR_PIN   14

#elif defined(CONFIG_OGXM_BOARD_ADAFRUIT_FEATHER)
    #define OGXM_BOARD          ADAFRUIT_FEATHER
    #define RGB_PWR_PIN         20
    #define RGB_PXL_PIN         21 

    #define PIO_USB_DP_PIN      16 // DM = 17
    #define LED_INDICATOR_PIN   13
    #define VCC_EN_PIN          18

#elif defined(CONFIG_OGXM_BOARD_INTERNAL_4CH)
    #define OGXM_BOARD          INTERNAL_4CH_I2C
    #define PIO_USB_DP_PIN      16 // DM = 17 
    #define FOUR_CH_ENABLED     1
    #define I2C_SDA_PIN         10
    #define I2C_SCL_PIN         11
    #define SLAVE_ADDR_PIN_1    20
    #define SLAVE_ADDR_PIN_2    21

#elif defined(CONFIG_OGXM_BOARD_EXTERNAL_4CH)
    #define OGXM_BOARD          EXTERNAL_4CH_I2C
    #define RGB_PXL_PIN         16 
    #define FOUR_CH_ENABLED     1
    #define PIO_USB_DP_PIN      10 // DM = 11
    #define I2C_SDA_PIN         6
    #define I2C_SCL_PIN         7
    #define SLAVE_ADDR_PIN_1    13
    #define SLAVE_ADDR_PIN_2    14

#elif defined(CONFIG_OGXM_BOARD_ESP32_BLUEPAD32_I2C)
    #define OGXM_BOARD          ESP32_BLUEPAD32_I2C
    #define I2C_SDA_PIN         18
    #define I2C_SCL_PIN         19
    #define UART0_TX_PIN        16
    #define UART0_RX_PIN        17
    #define MODE_SEL_PIN        21
    #define ESP_PROG_PIN        20 // ESP32 IO0
    #define ESP_RST_PIN         8  // ESP32 EN

    #if MAX_GAMEPADS > 1
        #undef MAX_GAMEPADS
        #define MAX_GAMEPADS 1
    #endif

#elif defined(CONFIG_OGXM_BOARD_ESP32_BLUERETRO_I2C)
    #define OGXM_BOARD          ESP32_BLUERETRO_I2C
    #define I2C_SDA_PIN         18
    #define I2C_SCL_PIN         19
    #define UART0_TX_PIN        16
    #define UART0_RX_PIN        17
    #define MODE_SEL_PIN        21
    #define ESP_PROG_PIN        20 // ESP32 IO0
    #define ESP_RST_PIN         8  // ESP32 EN

    #if MAX_GAMEPADS > 1
        #undef MAX_GAMEPADS
        #define MAX_GAMEPADS 1
    #endif

#elif defined(CONFIG_OGXM_BOARD_RP2350_USB_A)
    #define OGXM_BOARD          RP2350_USB_A
    /* D+ = GP12, D- = GP13 on the genuine Waveshare RP2350-USB-A. Both are overridable
     * (-DPIO_USB_DP_PIN=.. / -DPIO_USB_SWAP_DP_DM=1, see CMakeLists.txt) because clone boards
     * are not guaranteed to match this pinout — see README "Clones and knock-offs are not
     * supported". Two distinct clone failure modes, diagnosed differently:
     *  1) Host port never reacts at all (no mount, no brief connect blip, nothing): suspect the
     *     RP2350 pad drive, not the pin numbers. This MCU's internal pull-downs are too weak for
     *     pio_usb's bit-banged host-mode line sensing (see hardware/README.md's Pico 2 note) —
     *     the genuine Waveshare board has external 4.7k pull-downs from D+/D- to GND that many
     *     clones omit. Add them at the USB-A connector's data pins; no firmware setting fixes a
     *     missing resistor.
     *  2) Host port reacts (powers the device / brief mount / connect IRQ fires) but never
     *     enumerates: the data pair is likely swapped on the clone's silkscreen vs. this pinout.
     *     pio_usb's "swap" only changes which *neighboring* pin is D- (pin_dp-1 instead of
     *     pin_dp+1) — it does not flip which pin is D+. To fully reverse a swapped pair, change
     *     BOTH: -DPIO_USB_DP_PIN=13 -DPIO_USB_SWAP_DP_DM=1 (makes GP13 the new D+, GP12 the new
     *     D-). Setting PIO_USB_SWAP_DP_DM alone with PIO_USB_DP_PIN still 12 does not swap D+/D-;
     *     it points D- at GP11 instead, which is very unlikely to be what a clone needs. */
    #ifndef PIO_USB_DP_PIN
    #define PIO_USB_DP_PIN      12
    #endif
    #define RGB_PXL_PIN         16
    #ifndef PIO_USB_SWAP_DP_DM
    #define PIO_USB_SWAP_DP_DM  0
    #endif

#elif defined(CONFIG_OGXM_BOARD_RP2350_ZERO)
    #define OGXM_BOARD          RP2350_ZERO
    #define PIO_USB_DP_PIN      10 // DM = 11
    #define RGB_PXL_PIN         16

#elif defined(CONFIG_OGXM_BOARD_RP2040_XIAO)
    #define OGXM_BOARD          RP2040_XIAO
    #define RGB_PXL_PIN         12
    #define RGB_PWR_PIN         11
    #define PIO_USB_DP_PIN      0  // DM = 1
    #define LED_INDICATOR_PIN   17

#elif defined(CONFIG_OGXM_BOARD_RP2354)
    #define OGXM_BOARD          RP2354
    /* RP2350 + Pi Radio Module 2 (CYW43439): same PIO USB host pins as Pico 2 W. */
    #define PIO_USB_DP_PIN      0  // D+ = GP0, D- = GP1
    #ifndef PIO_USB_SWAP_DP_DM
    #define PIO_USB_SWAP_DP_DM  0
    #endif
    #define LED_INDICATOR_PIN   25

#else
    #error "Invalid OGXMini board selected"

#endif

#if defined(I2C_SDA_PIN)
    #define I2C_BAUDRATE 400 * 1000
    #define I2C_PORT    ((I2C_SDA_PIN == 2 ) || \
                         (I2C_SDA_PIN == 6 ) || \
                         (I2C_SDA_PIN == 10) || \
                         (I2C_SDA_PIN == 14) || \
                         (I2C_SDA_PIN == 18) || \
                         (I2C_SDA_PIN == 26)) ? i2c1 : i2c0
#endif // defined(I2C_SDA_PIN)

#if defined(PIO_USB_DP_PIN)
    /* Boards that don't set this above (PI_PICOW / RP2354 / RP2350_USB_A do) never need the
     * swap, so default it here rather than requiring every board section to declare it. */
    #ifndef PIO_USB_SWAP_DP_DM
    #define PIO_USB_SWAP_DP_DM  0
    #endif

    #if PIO_USB_SWAP_DP_DM
        #define PIO_USB_CONFIG_PINOUT PIO_USB_PINOUT_DMDP
    #else
        #define PIO_USB_CONFIG_PINOUT PIO_USB_PINOUT_DPDM
    #endif

    #define PIO_USB_CONFIG { \
        PIO_USB_DP_PIN, \
        PIO_USB_TX_DEFAULT, \
        PIO_SM_USB_TX_DEFAULT, \
        PIO_USB_DMA_TX_DEFAULT, \
        PIO_USB_RX_DEFAULT, \
        PIO_SM_USB_RX_DEFAULT, \
        PIO_SM_USB_EOP_DEFAULT, \
        NULL, \
        PIO_USB_DEBUG_PIN_NONE, \
        PIO_USB_DEBUG_PIN_NONE, \
        false, \
        PIO_USB_CONFIG_PINOUT \
    }
#endif // defined(PIO_USB_DP_PIN)

/* Boards that run the Pico W firmware path (CYW43439 BT + optional PIO USB host mux). */
#if defined(CONFIG_OGXM_BOARD_PI_PICOW) || defined(CONFIG_OGXM_BOARD_PI_PICO2W) || \
    (defined(CONFIG_OGXM_BOARD_RP2354) && defined(CONFIG_EN_BLUETOOTH))
    #define OGXM_BOARD_USES_PICO_W_FIRMWARE 1
#endif

#endif // _BOARD_CONFIG_H_
