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
 * @file    macros.h
 * @brief   Defining all macros at one place
 *
 * This header file defines all macros required for functionality
 *
 * @author  Jithendra H S
 * @date    10-17-2023
 *
 */
#ifndef MACROS_H_
#define MACROS_H_

#define ZERO (0)          /**< Number zero */
#define ONE (1)           /**< Number one */
#define TWO (2)           /**< Number two */
#define THREE (3)         /**< Number Three */
#define FOUR (4)          /**< Number Four */
#define EIGHT (8)          /**< Number Eight */
#define TEN (10)          /**< Number Ten */
#define FOURTEEN (14)     /**< Number Fourteen */
#define SIXTEEN (16)     /**< Number Fourteen */
#define SEVENTEEN (17)    /**< Number Seventeen */
#define TWENTY_FOUR (24)  /**< Number twenty-four */
#define TWENTY_FIVE (25)  /**< Number twenty-five */
#define FORTY_NINE (49)   /**< Number forty-nine */
#define FIFTY (50)        /**< Number fifty */
#define NINTY (90)        /**< Number Ninty */
#define HUNDRED (100)     /**< Number hundred */
#define FIVE_HUNDRED (500)/**< Number Five hundred */
#define THOUSAND (1000)   /**< Number Thousand */

#define DIM_LED (1)           // Signal to dim the LED
#define BRIGHT_LED (0)        // Signal to brighten the LED
#define CHANGE_LED (1)        // Signal to change LED state
#define KEEP_LED_BRIGHT (2)   // Signal to keep the LED bright
#define KEEP_LED_DIM (3)      // Signal to keep the LED dim

#endif /* MACROS_H_ */
