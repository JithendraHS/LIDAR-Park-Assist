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
 * @file   log.h
 * @brief Abstracting away from PRINTF
 *
 *
 * @author  Jithendra H S
 * @date    09-30-2023
 *
 */


#ifndef LOG_H_
#define LOG_H_
#include "fsl_debug_console.h"

#ifdef DEBUG
#define LOG PRINTF  // Implementing LOG as abstraction of PRINTF in DEBUG mode
#else
#define LOG(...)    // This will be ignored in RELEASE mode
#endif
#endif /* LOG_H_ */
