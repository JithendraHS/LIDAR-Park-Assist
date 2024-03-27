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
 * @file    touch.h
 * @brief   Touch sense functionality implementation using KL25Z LEDs
 *
 * This header file defines functions and constants related to touch sense
 * functionality using KL25Z LEDs. It includes function prototypes for
 * initializing the touch sense module and fetching touch sense values.
 *
 * @author  Jithendra H S
 * @date    10-17-2023
 *
 */
#ifndef TOUCH_H_
#define TOUCH_H_

#include <log.h>

/**
 * @brief   Initializing Touch sense by enabling clock gating into the interface
 *          and setting bits in the general control and status register.
 *
 * @return  Nothing
 *
 *  @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M
 *             based Microcontrollers", chapter 2.
 */
void Touch_Init();

/**
 * @brief   Fetching Touch sense value from TSI DATA register.
 *
 * @return  Touch sense value after adjusting for offset.
 *
 *  @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M
 *             based Microcontrollers", chapter 2.
 */
int Touch_Scan_LH();

#endif /* TOUCH_H_ */
