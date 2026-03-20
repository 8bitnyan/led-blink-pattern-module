#ifndef LED_BLINK_PATTERN_H
#define LED_BLINK_PATTERN_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the LED blink pattern module.
 * Configures initial state and ensures LED starts OFF.
 */
void led_blink_pattern_init(void);

/**
 * Main loop handler for the LED blink pattern.
 * Call repeatedly from the main loop. Uses non-blocking timing
 * to produce a 1-2-3 repeating blink pattern.
 */
void led_blink_pattern_loop(void);

#ifdef __cplusplus
}
#endif

#endif /* LED_BLINK_PATTERN_H */