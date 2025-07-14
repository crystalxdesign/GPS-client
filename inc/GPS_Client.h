/*
 * GPSClientProject for Raspberry Pi Pico
 *
 * @version     1.0.0
 * @author      <YOU>
 * @copyright   2024
 * @licence     MIT
 *
 */
#ifndef _GPSClientProject_MAIN_HEADER_
#define _GPSClientProject_MAIN_HEADER_

/*
 * C++ HEADERS
 */
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstring>

/*
 * PICO HEADERS
 */
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#ifdef __cplusplus
extern "C" {
#endif
#define FIRMWARE_VERSION "v1.0.1"
#define LED_BRIGHTNESS 50
// Comment out the following RP2030_Zero definition if using a standard pico
// Uncomment if using a pico zero. See diagrams for differences.
#define RP2030_Zero
#ifdef RP2030_Zero
#define BUTTON_PIN  8
#define WS2812_GPIO 16
#else
#define BUTTON_PIN  16
#define WS2812_GPIO 23
#endif
#include "TinyGPSPlus.h"
#include "Nokia5110.h"
#include "images.h"
#include "WS2812.hpp"
#include "ST7567_font.h"
#include "ST7567_FB.h"
/*
 * Usual header code here
 */

#ifdef __cplusplus
}
#endif

#endif // _GPSClientProject_MAIN_HEADER_
