/**
 * LED Blink Pattern Module
 *
 * Blinks an LED in a repeating 1-2-3 pattern:
 *   - 1 blink, pause
 *   - 2 blinks, pause
 *   - 3 blinks, pause
 *   - repeat
 */

#include "led_blink_pattern.h"
#include "modules_config.h"

/* Validate required parameters */
#if !defined(LED_PATTERN_PORT) || !defined(LED_PATTERN_PIN)
#error "led_blink_pattern requires LED_PATTERN_PORT and LED_PATTERN_PIN"
#endif

#ifndef LED_PATTERN_BLINK_MS
#define LED_PATTERN_BLINK_MS 200
#endif

#ifndef LED_PATTERN_GAP_MS
#define LED_PATTERN_GAP_MS 200
#endif

#ifndef LED_PATTERN_PAUSE_MS
#define LED_PATTERN_PAUSE_MS 1000
#endif

/* Pattern: blink 1x, 2x, 3x, repeat */
#define PATTERN_LENGTH 3
static const uint8_t pattern_counts[PATTERN_LENGTH] = { 1, 2, 3 };

/* State machine states */
typedef enum {
    STATE_BLINK_ON,
    STATE_BLINK_OFF,
    STATE_PAUSE
} blink_state_t;

/* Module state */
static blink_state_t state = STATE_BLINK_ON;
static uint32_t last_tick = 0;
static uint8_t current_blink = 0;     /* which blink we're on within current count */
static uint8_t pattern_index = 0;     /* index into pattern_counts[] */

void led_blink_pattern_init(void) {
    HAL_GPIO_WritePin(LED_PATTERN_PORT, LED_PATTERN_PIN, GPIO_PIN_RESET);
    state = STATE_BLINK_ON;
    last_tick = HAL_GetTick();
    current_blink = 0;
    pattern_index = 0;
}

void led_blink_pattern_loop(void) {
    uint32_t now = HAL_GetTick();
    uint32_t elapsed = now - last_tick;

    switch (state) {
    case STATE_BLINK_ON:
        if (elapsed == 0) {
            /* First entry: turn LED on */
            HAL_GPIO_WritePin(LED_PATTERN_PORT, LED_PATTERN_PIN, GPIO_PIN_SET);
        }
        if (elapsed >= LED_PATTERN_BLINK_MS) {
            /* Blink duration complete: turn off */
            HAL_GPIO_WritePin(LED_PATTERN_PORT, LED_PATTERN_PIN, GPIO_PIN_RESET);
            current_blink++;
            last_tick = now;
            if (current_blink >= pattern_counts[pattern_index]) {
                /* All blinks for this count done, pause */
                state = STATE_PAUSE;
            } else {
                /* More blinks needed, gap then blink again */
                state = STATE_BLINK_OFF;
            }
        }
        break;

    case STATE_BLINK_OFF:
        if (elapsed >= LED_PATTERN_GAP_MS) {
            /* Gap complete: start next blink */
            state = STATE_BLINK_ON;
            last_tick = now;
        }
        break;

    case STATE_PAUSE:
        if (elapsed >= LED_PATTERN_PAUSE_MS) {
            /* Pause complete: advance to next pattern count */
            pattern_index = (pattern_index + 1) % PATTERN_LENGTH;
            current_blink = 0;
            state = STATE_BLINK_ON;
            last_tick = now;
        }
        break;
    }
}