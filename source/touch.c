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
 * @file    touch.c
 * @brief   Touch sense functionality implementation using KL25Z LEDs
 *
 * This source file contains the implementation of functions to control the
 * touch sense functionality using KL25Z microcontroller. It includes functions
 * to initialize the touch sense module and fetch touch sense values from the
 * TSI (Touch Sensing Input) DATA register.
 *
 * @author  Jithendra H S
 * @date    09-30-2023
 *
 */

#include <touch.h>
#include <macros.h>

#define TOUCH_OFFSET 600            // Offset value to be subtracted due to noise
#define TOUCH_DATA (TSI0->DATA & 0xFFFF)  // Macro for extracting the count from
                                    // data register
/**
 * @brief   Initializing Touch sense by enabling clock gating into the interface
 *          and setting bits in the general control and status register.
 *
 * @return  Nothing
 *
 *  @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M
 *          based Microcontrollers", chapter 2.
 */
void Touch_Init()
{
    /* Enable the clock for the TSI module */
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

    /* Configure TSI module settings */
    TSI0->GENCS = TSI_GENCS_MODE(0u) |     // Operating in non-noise mode
                  TSI_GENCS_REFCHRG(0u) |   // Reference oscillator charge and
                                          // discharge value 500nA
                  TSI_GENCS_DVOLT(0u) |     // Oscillator voltage rails set to default
                  TSI_GENCS_EXTCHRG(0u) |   // Electrode oscillator charge and
                                          // discharge value 500nA
                  TSI_GENCS_PS(0u) |        // Frequency clock divided by one
                  TSI_GENCS_NSCN(31u) |     // Scanning the electrode 32 times
                  TSI_GENCS_TSIEN_MASK |    // Enabling the TSI module
                  TSI_GENCS_EOSF_MASK;      // Writing one to clear the end of scan flag
}

/**
 * @brief   Fetching Touch sense value from TSI DATA register.
 *
 * @return  Touch sense value after adjusting for offset.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M
 *             based Microcontrollers", chapter 2.
 */
int Touch_Scan_LH()
{
    unsigned int scan = 0;

    /* Assigning value from TSI channel 10 */
    TSI0->DATA = TSI_DATA_TSICH(10u);

    /* Software trigger to start the scan */
    TSI0->DATA |= TSI_DATA_SWTS_MASK;

    /* Waiting for the scan to complete 32 times */
    while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK))
        ;

    /* Getting touch sense value */
    scan = TOUCH_DATA;

    /* Clearing the end of scan flag */
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;

    /* Returning the touch sense value after adjusting for offset */
    return (scan - TOUCH_OFFSET);
}
