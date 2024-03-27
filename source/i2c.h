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
 * @file i2c.h
 * @brief Header file for I2C communication module.
 * @author Jithendra H S
 * @date 12-Dec-2023
 *
 */

#include <stdint.h>

/**
 * @brief Macro to set I2C module to master mode and generate a start condition.
 */
#define I2C_M_START     I2C1->C1 |= I2C_C1_MST_MASK

/**
 * @brief Macro to clear the master mode, generating a stop condition.
 */
#define I2C_M_STOP      I2C1->C1 &= ~I2C_C1_MST_MASK

/**
 * @brief Macro to generate a repeated start condition.
 */
#define I2C_M_RSTART    I2C1->C1 |= I2C_C1_RSTA_MASK

/**
 * @brief Macro to set I2C module to transmit mode.
 */
#define I2C_TRAN        I2C1->C1 |= I2C_C1_TX_MASK

/**
 * @brief Macro to set I2C module to receive mode.
 */
#define I2C_REC         I2C1->C1 &= ~I2C_C1_TX_MASK

/**
 * @brief Macro to wait until the I2C bus is busy.
 */
#define BUSY_ACK        while(I2C1->S & 0x01)

/**
 * @brief Macro to wait until the I2C transmission is complete.
 */
#define TRANS_COMP      while(!(I2C1->S & 0x80))

/**
 * @brief Macro to wait using a custom I2C wait function.
 */
#define I2C_WAIT        i2c_wait();

/**
 * @brief Macro to set NACK (Not Acknowledge) in the I2C module.
 */
#define NACK            I2C1->C1 |= I2C_C1_TXAK_MASK

/**
 * @brief Macro to clear NACK, enabling ACK (Acknowledge) in the I2C module.
 */
#define ACK             I2C1->C1 &= ~I2C_C1_TXAK_MASK

/**
 * @brief Function to initialize the I2C module.\
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_init(void);

/**
 * @brief Function to generate a start condition on the I2C bus.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_start(void);

/**
 * @brief Function to set up I2C for reading from a specific device and address.
 * @param dev The I2C device address (7-bit).
 * @param address The register address to read from.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_read_setup(uint8_t dev, uint8_t address);

/**
 * @brief Function to perform a repeated I2C read operation.
 * @param isLastRead Indicates whether this is the last read in the sequence.
 *                   If true, a NACK (Not Acknowledge) is sent after the read.
 *                   If false, an ACK (Acknowledge) is sent after the read.
 * @return The data read from the I2C bus.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
uint8_t i2c_repeated_read(uint8_t isLastRead);

/**
 * @brief Function to read a byte from a specific address of an I2C device.
 * @param dev The I2C device address (7-bit) to communicate with.
 * @param address The register address from which to read the byte.
 * @return The data byte read from the specified address of the I2C device.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/**
 * @brief Function to write a byte of data to a specific address of an I2C device.
 * @param dev The I2C device address (7-bit) to communicate with.
 * @param address The register address where the data will be written.
 * @param data The byte of data to be written to the specified address.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);
