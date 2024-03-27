/*******************************************************************************
 * Copyright (C) 2023 by Jithendra H S
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Jithendra H S and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/
/**
 * @file task.c
 * @brief Source file for task management module.
 * @author Jithendra H S
 * @date 12-Dec-2023
 */

#include "task.h"
#include "fsl_debug_console.h"
#include "i2c.h"
#include "pin_mux.h"
#include <touch.h>
#include "led.h"
#include "macros.h"

#define LIDAR_I2C_ADDRESS (0x20)
#define DISTANCE_BYTE_HIGH (0x01)
#define DISTANCE_BYTE_LOW  (0x00)
/* Task handles for accessing the tasks later if needed */
TaskHandle_t forward_handle;
TaskHandle_t reverse_handle;

/**
 * @struct LED_RGB
 * @brief Structure to represent RGB LED settings based on touch value range.
 */
struct LED_RGB {
    uint32_t min_sense_val; /**< Minimum touch sensor value for this range */
    uint32_t max_sense_val; /**< Maximum touch sensor value for this range */
    uint32_t delay;         /**< Delay time */
    uint8_t r_dim;          /**< Red LED dim status */
    uint8_t g_dim;          /**< Green LED dim status */
    uint8_t b_dim;          /**< Blue LED dim status */
    uint8_t r_mode;         /**< Red LED mode */
    uint8_t g_mode;         /**< Green LED mode */
    uint8_t b_mode;         /**< Blue LED mode */
};

/**
 * @var RGB_Table
 * @brief Array of LED_RGB structures representing translation from touch value to color.
 */
struct LED_RGB RGB_Table[] = {
    { 0, 90, 1, DIM_LED, DIM_LED, DIM_LED, CHANGE_LED, KEEP_LED_DIM, KEEP_LED_DIM },
    { 91, 180, 0, DIM_LED, DIM_LED, DIM_LED, CHANGE_LED, CHANGE_LED, KEEP_LED_DIM },
    { 180, 720, 0, DIM_LED, DIM_LED, DIM_LED, KEEP_LED_DIM, CHANGE_LED, KEEP_LED_DIM },
    { 0, 0, 0, DIM_LED, DIM_LED, DIM_LED, KEEP_LED_DIM, KEEP_LED_DIM, KEEP_LED_DIM },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
};

/**
 * @brief Function to handle forward gear logic.
 *
 * This function is responsible for managing the logic associated with the forward gear state.
 *
 * @param pvParameters Pointer to task parameters (not used).
 */
void forward(void *pvParameters) {
    uint8_t reverse_gear_applied = 0; /**< Flag indicating if reverse gear is applied. */
    uint8_t percentage = 0; /**< Percentage value calculated based on touch sensor. */
    int touch_val = 0; /**< Touch sensor value. */
    int touch_sensed = 1; /**< Flag indicating if touch has been sensed. */

    while (ONE) {
        // Read touch sensor value
        touch_val = Touch_Scan_LH();
        LOG("Touch value %d\n\r", touch_val);

        // Handle forward gear logic
        if (touch_val > ZERO && !touch_sensed) {
            touch_sensed = ONE;
            reverse_gear_applied = !reverse_gear_applied;
            if (!reverse_gear_applied) {
                LOG("Gear shifted to forward\n\r");
                dim_led(percentage, RGB_Table[3].r_dim, RGB_Table[3].g_dim,
                        RGB_Table[3].b_dim, RGB_Table[3].r_mode,
                        RGB_Table[3].g_mode, RGB_Table[3].b_mode);
                vTaskSuspend(reverse_handle);
            } else {
                LOG("Gear shifted to reverse\n\r");
            }
        }

        if (touch_val < ZERO && touch_sensed) {
            touch_sensed = ZERO;
            LOG("touch sense reset\n\r");
        }

        // Resume reverse task if reverse gear is applied
        if (reverse_gear_applied) {
            vTaskResume(reverse_handle);
        }

        // Delay to control task execution frequency
        vTaskDelay(TEN / portTICK_PERIOD_MS);
    }
}


/**
 * @brief Function to handle reverse gear logic.
 *
 * This function is responsible for managing the logic associated with the reverse gear state.
 *
 * @param pvParameters Pointer to task parameters (not used).
 */
void reverse(void *pvParameters) {
    uint16_t distance = 0; /**< Distance value read from the I2C sensor. */
    uint8_t percentage = 0; /**< Percentage value calculated based on distance. */

    while (ONE) {
        // Read distance from I2C sensor
        distance = (uint16_t)i2c_read_byte(LIDAR_I2C_ADDRESS, DISTANCE_BYTE_HIGH);
        distance = distance << EIGHT;
        vTaskDelay(TEN / portTICK_PERIOD_MS);
        distance |= (uint16_t)i2c_read_byte(LIDAR_I2C_ADDRESS, DISTANCE_BYTE_LOW);
        LOG("Distance : %d\n\r", distance);

        // Determine LED settings based on distance value
        for (int i = 0; i < THREE; i++) {
            if (distance > RGB_Table[i].min_sense_val && distance < RGB_Table[i].max_sense_val) {
                percentage = ((float)(distance % NINTY) / NINTY) * HUNDRED;
                dim_led(percentage, RGB_Table[i].r_dim, RGB_Table[i].g_dim,
                        RGB_Table[i].b_dim, RGB_Table[i].r_mode,
                        RGB_Table[i].g_mode, RGB_Table[i].b_mode);

                // Add delay if specified by the RGB_Table configuration
                if (RGB_Table[i].delay) {
                    vTaskDelay((percentage * TEN) / portTICK_PERIOD_MS);
                    dim_led(percentage, RGB_Table[3].r_dim, RGB_Table[3].g_dim,
                            RGB_Table[3].b_dim, RGB_Table[3].r_mode,
                            RGB_Table[3].g_mode, RGB_Table[3].b_mode);
                }
                break;
            }
        }

        // Delay to control task execution frequency
        vTaskDelay(TEN / portTICK_PERIOD_MS);
    }
}

