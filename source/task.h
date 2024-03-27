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
 * @file task.h
 * @brief Task management header file.
 *
 * This file contains the definitions and prototypes related to tasks in the system.
 * Tasks include 'forward' and 'reverse' tasks, each with their own priority.
 *
 * @author  Jithendra H S
 * @date    13-12-2023
 *
 */

#ifndef TASK_H_
#define TASK_H_

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define STACK_SIZE (512) // size of stack for each task

/* Task priorities. */
#define forward_task_PRIORITY (configMAX_PRIORITIES - 1)
#define reverse_task_PRIORITY (configMAX_PRIORITIES - 1)

/* Task handles for accessing the tasks later if needed */
extern TaskHandle_t forward_handle; /**< Task handle for the 'forward' task. */
extern TaskHandle_t reverse_handle; /**< Task handle for the 'reverse' task. */

/* Task Prototypes */
/**
 * @brief Function to handle forward gear logic.
 * @param pvParameters Pointer to task parameters (not used).
 */
void forward(void *pvParameters);

/**
 * @brief Function to handle reverse gear logic.
 * @param pvParameters Pointer to task parameters (not used).
 */
void reverse(void *pvParameters);

#endif /* TASK_H_ */
