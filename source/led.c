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
 * @file   led.c
 * @brief  Train tracking Implementation using KL25Z LEDs
 *
 * This source file contains the implementation of functions to control train
 * tracking using KL25Z LEDs. It includes initialization of the RGB LEDs and
 * functions to dim and change the LED states based on control signals.
 *
 * @author  Jithendra H S
 * @date    10-17-2023
 *
 */

#include "led.h"        // Include the LED header file
#include <macros.h>     // Include the MACROS defined in the header file

#define RED_LED_SHIFT   (18)  // Red LED pin on port B
#define GREEN_LED_SHIFT (19)  // Green LED pin on port B
#define BLUE_LED_SHIFT  (1)   // Blue LED pin on port D

#define PWM_PERIOD (48000)    // PWM period for LED control
#define FULL_ON (PWM_PERIOD - 1) // Full brightness value for PWM
#define FULL_OFF (0)           // No brightness value for PWM (LED off)
#define ONE_PERCENT (0.01)    // one percent required for duty cycle calculation

/**
 * @brief Initializes the RGB LED PWM functionality.
 *
 * This function configures the necessary pins and timers for PWM control
 * of the RGB LEDs on the KL25Z microcontroller.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M
 *     based Microcontrollers", chapter 7.
 */
void Init_RGB_LED_PWM() {
	// Enable clock to port B and port D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	// Set pin to PWMs with suitable MUX values
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(THREE);
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(THREE);
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(FOUR);

	// Configure TPM by enabling clock gating to peripherals
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;

	// Set clock source for TPM: 48 MHz
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(ONE) | SIM_SOPT2_PLLFLLSEL_MASK);

	// Load the counter
	TPM0->MOD = PWM_PERIOD - ONE;
	TPM2->MOD = PWM_PERIOD - ONE;

	// Set TPM count direction to up with a divide by 2 prescaler
	TPM0->SC = TPM_SC_PS(ONE);
	TPM2->SC = TPM_SC_PS(ONE);

	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(THREE);
	TPM2->CONF |= TPM_CONF_DBGMODE(THREE);

	// Set channels to edge-aligned low-true PWM
	TPM0->CONTROLS[ONE].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[ZERO].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[ONE].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;

	// Set initial duty cycle to zero percentage
	TPM0->CONTROLS[ONE].CnV = ZERO;
	TPM2->CONTROLS[ZERO].CnV = ZERO;
	TPM2->CONTROLS[ONE].CnV = ZERO;

	// Start TPM
	TPM0->SC |= TPM_SC_CMOD(ONE);
	TPM2->SC |= TPM_SC_CMOD(ONE);
}

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
		int dim_blue, int change_red, int change_green, int change_blue) {
	// Change red LED intensity
	if (change_red == CHANGE_LED) {
		if (dim_red) {
			// Dimming red LED
			TPM2->CONTROLS[ZERO].CnV = PWM_PERIOD
					- ((PWM_PERIOD - ONE)
							* (ONE_PERCENT * duty_cycle_percentage));
		} else {
			// Brightening red LED
			TPM2->CONTROLS[ZERO].CnV = (PWM_PERIOD - ONE)
					* (ONE_PERCENT * duty_cycle_percentage);
		}
	} else if (change_red == KEEP_LED_BRIGHT) {
		// keep led bright while transition
		TPM2->CONTROLS[ZERO].CnV = PWM_PERIOD - ONE;
	} else if (change_red == KEEP_LED_DIM) {
		// keep led dim while transition
		TPM2->CONTROLS[ZERO].CnV = ZERO;
	}

	// Change green LED intensity
	if (change_green == CHANGE_LED) {
		if (dim_green) {
			// Dimming green LED
			TPM2->CONTROLS[ONE].CnV = PWM_PERIOD
					- ((PWM_PERIOD - ONE)
							* (ONE_PERCENT * duty_cycle_percentage));
		} else {
			// Brightening green LED
			TPM2->CONTROLS[ONE].CnV = (PWM_PERIOD - ONE)
					* (ONE_PERCENT * duty_cycle_percentage);
		}
	} else if (change_green == KEEP_LED_BRIGHT) {
		// keep led bright while transition
		TPM2->CONTROLS[ONE].CnV = PWM_PERIOD - ONE;
	} else if (change_green == KEEP_LED_DIM) {
		// keep led dim while transition
		TPM2->CONTROLS[ONE].CnV = ZERO;
	}

	// Change blue LED intensity
	if (change_blue == CHANGE_LED) {
		if (dim_blue) {
			// Dimming blue LED
			TPM0->CONTROLS[ONE].CnV = PWM_PERIOD
					- ((PWM_PERIOD - ONE)
							* (ONE_PERCENT * duty_cycle_percentage));
		} else {
			// Brightening blue LED
			TPM0->CONTROLS[ONE].CnV = (PWM_PERIOD - ONE)
					* (ONE_PERCENT * duty_cycle_percentage);
		}
	} else if (change_blue == KEEP_LED_BRIGHT) {
		// keep led bright while transition
		TPM0->CONTROLS[ONE].CnV = PWM_PERIOD - ONE;
	} else if (change_blue == KEEP_LED_DIM) {
		// keep led dim while transition
		TPM0->CONTROLS[ONE].CnV = ZERO;
	}
}

/**
 * @brief Sets the intensity of RGB LEDs using PWM.
 * @param red   Intensity value for the red LED (0-255).
 * @param green Intensity value for the green LED (0-255).
 * @param blue  Intensity value for the blue LED (0-255).
 */
void lit_led(int red, int green, int blue){
    // Calculate PWM values based on input intensity values

    // Set the PWM value for the red LED
    TPM2->CONTROLS[ZERO].CnV = (PWM_PERIOD - ONE) * ((float)red / 255.0f);

    // Set the PWM value for the green LED
    TPM2->CONTROLS[ONE].CnV = (PWM_PERIOD - ONE) * ((float)green / 255.0f);

    // Set the PWM value for the blue LED
    TPM0->CONTROLS[ONE].CnV = (PWM_PERIOD - ONE) * ((float)blue / 255.0f);
}
