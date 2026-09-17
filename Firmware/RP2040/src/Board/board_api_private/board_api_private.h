#ifndef BOARD_API_PRIVATE_H
#define BOARD_API_PRIVATE_H

#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>

namespace board_api_bt {
    void init() __attribute__((weak));
    void set_led(bool state) __attribute__((weak));
}

namespace board_api_led {
    void init() __attribute__((weak));
    void set_led(bool state) __attribute__((weak));
}

namespace board_api_rgb {
    void init() __attribute__((weak));
    void set_led(uint8_t r, uint8_t g, uint8_t b) __attribute__((weak));
}

namespace board_api_usbh {
    void init() __attribute__((weak));
    bool host_connected() __attribute__((weak));
    /** Stop edge IRQs on D+/D− before PIO USB reclaims pins (avoids IO_IRQ storm / BT stall). */
    void suspend_line_irq();
    /** While PIO owns D+/D−, update the same flag GPIO IRQs used (from hcd_port_connect_status). */
    void store_host_line_connected(bool connected);
    /** After tuh_deinit / PIO release path, re-enable D+/D− edge IRQs for the next cable attach. */
    void enable_host_line_irq_monitoring();
    /** Stop PIO USB host (SOF timer, tuh_deinit) before core reset / reboot. Standard boards. */
    void stop_pio_usb_host() __attribute__((weak));
    /**
     * True while pio_usb_host_frame() is already being serviced at ~1 kHz by a hardware
     * repeating_timer IRQ (see PicoW.cpp / Standard.cpp). pio_usb_host_frame() is not reentrant —
     * callers that would otherwise invoke it manually from main-loop code (e.g. HostManager's
     * send_feedback path, tuh_xinput's TX-complete busy-wait) must skip that call while this is
     * true, or the IRQ can land mid-frame inside the manual call and corrupt PIO-USB's transfer
     * state (silent host freeze, no recovery). Weak stub defaults to false for boards/paths with
     * no hardware SOF timer, where the manual call is still required.
     */
    bool sof_timer_active() __attribute__((weak));
}

#endif // BOARD_API_PRIVATE_H
