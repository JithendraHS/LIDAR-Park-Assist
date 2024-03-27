/*******************************************************************************
 * Copyright (C) 2023 by Jithendra H S
 *
 * Redistribution, modification, or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Jithendra H S and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file   led.h
 * @brief  Train tracking using KL25Z LEDs
 *
 * This header file defines functions and constants for tracking train
 * using KL25Z LEDs. It includes initialization functions and control functions
 * for dimming and changing LED states based on specific signals.
 *
 * @author  Jithendra H S
 * @date    10-17-2023
 *
 */

#ifndef LED_H_
#define LED_H_

#include <MKL25Z4.h>   // Include the MKL25Z4 microcontroller header file
#include <stdint.h>     // Include the standard integer data types header

/**
 * @brief Initializes the RGB LED PWM functionality.
 *
 * This function configures the necessary pins and timers for PWM control
 * of the RGB LEDs on the KL25Z microcontroller.
 *
 * * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M
 *  based Microcontrollers", chapter 7.
 */
void Init_RGB_LED_PWM();

/**
 * @brief Controls the brightness and state of the RGB LED.
 *
 * This function adjusts the duty cycle of the RGB LED based on the specified
 * duty cycle percentage and control signals for each color component.
 *
 * @param duty_cycle_percentage The percentage of duty cycle for the LED brightness.
 * @param dim_red Signal to dim the red LED (DIM_LED) or keep it bright (KEEP_LED_BRIGHT).
 * @param dim_green Signal to dim the green LED (DIM_LED) or keep it bright (KEEP_LED_BRIGHT).
 * @param dim_blue Signal to dim the blue LED (DIM_LED) or keep it bright (KEEP_LED_BRIGHT).
 * @param change_red Signal to change the state of the red LED (CHANGE_LED).
 * @param change_green Signal to change the state of the green LED (CHANGE_LED).
 * @param change_blue Signal to change the state of the blue LED (CHANGE_LED).
 */
void dim_led(int duty_cycle_percentage, int dim_red, int dim_green,
		int dim_blue, int change_red, int change_green, int change_blue);

/**
 * @brief Sets the intensity of RGB LEDs using PWM.
 * @param red   Intensity value for the red LED (0-255).
 * @param green Intensity value for the green LED (0-255).
 * @param blue  Intensity value for the blue LED (0-255).
 */
void lit_led(int red, int green, int blue);
#endif /* LED_H_ */
