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
 * @file i2c.c
 * @brief Source file for I2C communication module functions.
 * @author Jithendra H S
 * @date 12-Dec-2023
 */

#include <MKL25Z4.H>
#include "i2c.h"
int lock_detect=0;
int i2c_lock=0;

/**
 * @brief Initializes the I2C1 module.
 *
 * This function configures and initializes the I2C1 module for communication.
 * It sets up the necessary pins, baud rate, and enables the I2C1 module.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_init(void) {
    // Enable clock for I2C1 module
    SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;

    // Enable clock for Port E
    SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK);

    // Configure the pins for I2C1
    PORTE->PCR[0] |= PORT_PCR_MUX(6); // SDA
    PORTE->PCR[1] |= PORT_PCR_MUX(6); // SCL

    // Set the I2C1 frequency divider and multiplier for buad of 400k
    I2C1->F = (I2C_F_ICR(0x11) | I2C_F_MULT(0));

    // Enable the I2C1 module
    I2C1->C1 |= (I2C_C1_IICEN_MASK);

    // Enable High Drive Select
    I2C1->C2 |= (I2C_C2_HDRS_MASK);
}

/**
 * @brief Handles scenarios where the I2C bus is busy or locked.
 *
 * This function attempts to unlock the I2C bus and clear any potential bus lock.
 * It uses arbitration and transmission to reset the bus and then re-enables I2C.
 * Additionally, it sets flags for bus lock detection.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_busy(void) {
    // Variables for bus lock detection
    lock_detect = 0;
    i2c_lock = 1;

    // Disable I2C and configure for transmission
    I2C1->C1 &= ~I2C_C1_IICEN_MASK;
    I2C_TRAN;
    I2C_M_START;
    I2C1->C1 |= I2C_C1_IICEN_MASK;
    I2C1->C1 |= I2C_C1_MST_MASK;
    I2C1->C1 |= I2C_C1_TX_MASK;
    I2C1->D = 0xFF;

    // Wait for completion of the operation
    while ((I2C1->S & I2C_S_IICIF_MASK) == 0U) {
    }

    // Clear the interrupt flag
    I2C1->S |= I2C_S_IICIF_MASK;

    // Set the arbitration lost flag
    I2C1->S |= I2C_S_ARBL_MASK;

    // Disable I2C, set for transmission, and set master mode
    I2C1->C1 &= ~I2C_C1_IICEN_MASK;
    I2C1->C1 |= I2C_C1_TX_MASK;
    I2C1->C1 |= I2C_C1_MST_MASK;
    I2C1->C1 |= I2C_C1_IICEN_MASK;

    // Disable I2C, set master mode, clear master mode, clear transmission mode, and enable I2C
    I2C1->C1 &= ~I2C_C1_IICEN_MASK;
    I2C1->C1 |= I2C_C1_MST_MASK;
    I2C1->C1 &= ~I2C_C1_MST_MASK;
    I2C1->C1 &= ~I2C_C1_TX_MASK;
    I2C1->C1 |= I2C_C1_IICEN_MASK;

    // Set the interrupt flag and arbitration lost flag
    I2C1->S |= I2C_S_IICIF_MASK;
    I2C1->S |= I2C_S_ARBL_MASK;

    // Reset variables
    lock_detect = 0;
    i2c_lock = 1;
}


/**
 * @brief Waits for the I2C interrupt flag and handles bus lock scenarios.
 *
 * This function waits for the I2C interrupt flag to be set, indicating the completion
 * of the I2C operation. It also monitors for bus lock conditions, and if detected, invokes
 * the i2c_busy() function to reset the bus. The function clears the interrupt flag after use.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_wait(void) {
    // Reset bus lock detection counter
    lock_detect = 0;

    // Wait for the I2C interrupt flag or bus lock conditions
    while (((I2C1->S & I2C_S_IICIF_MASK) == 0) && (lock_detect < 200)) {
        lock_detect++;
    }

    // If bus lock conditions are detected, invoke i2c_busy() to reset the bus
    if (lock_detect >= 200) {
        i2c_busy();
    }

    // Clear the I2C interrupt flag
    I2C1->S |= I2C_S_IICIF_MASK;
}


/**
 * @brief Generates a start condition for I2C communication.
 *
 * This function sets the I2C module to transmit mode and generates a start condition
 * to initiate an I2C communication sequence.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_start(void) {
    // Set I2C to transmit mode
    I2C_TRAN;

    // Generate a start condition
    I2C_M_START;
}

/**
 * @brief Sets up the I2C module for reading data from a specific device and address.
 *
 * This function configures the I2C module to initiate a read operation from the specified
 * device and address. It sends the device address, the register address to read from, and
 * then restarts the communication to switch to reading mode.
 *
 * @param dev The device address.
 * @param address The register address to read from.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_read_setup(uint8_t dev, uint8_t address) {
    // Send device address
    I2C1->D = dev;
    I2C_WAIT;

    // Send register address
    I2C1->D = address;
    I2C_WAIT;

    // Restart and switch to reading mode
    I2C_M_RSTART;
    I2C1->D = (dev | 0x01);
    I2C_WAIT;

    // Set I2C to receive mode
    I2C_REC;
}

/**
 * @brief Performs a repeated I2C read operation.
 *
 * This function reads data from the I2C bus and optionally sends
 * an acknowledgment or not based on the isLastRead parameter.
 *
 * @param isLastRead Indicates whether this is the last read in the sequence.
 *                   If true, a NACK (Not Acknowledge) is sent after the read.
 *                   If false, an ACK (Acknowledge) is sent after the read.
 *
 * @return The data read from the I2C bus.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
uint8_t i2c_repeated_read(uint8_t isLastRead) {
    uint8_t data;

    // Reset the lock_detect flag.
    lock_detect = 0;

    // Set the I2C acknowledgement based on whether this is the last read.
    if (isLastRead) {
        NACK;  // Set NACK if it's the last read.
    } else {
        ACK;   // Set ACK if it's not the last read.
    }

    // Read data from the I2C data register.
    data = I2C1->D;

    // Wait for the I2C operation to complete.
    I2C_WAIT

    // If it's the last read, send a stop condition to end the I2C communication.
    if (isLastRead) {
        I2C_M_STOP;
    }

    // Read data from the I2C data register.
    data = I2C1->D;

    // Return the data read from the I2C bus.
    return data;
}

/**
 * @brief Reads a byte from a specific address of an I2C device.
 *
 * This function performs a complete I2C transaction to read a byte from a
 * specified address of the given I2C device.
 *
 * @param dev The I2C device address (7-bit) to communicate with.
 * @param address The register address from which to read the byte.
 *
 * @return The data byte read from the specified address of the I2C device.
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address) {
    uint8_t data;

    // Set I2C to transmit mode.
    I2C_TRAN;

    // Start the I2C communication.
    I2C_M_START;

    // Send the I2C device address for writing.
    I2C1->D = dev;
    I2C_WAIT

    // Send the register address to read from.
    I2C1->D =  address;
    I2C_WAIT

    // Repeated start for reading from the specified address.
    I2C_M_RSTART;

    // Send the I2C device address for reading.
    I2C1->D = (dev | 0x01);
    I2C_WAIT

    // Set I2C to receive mode.
    I2C_REC;

    // Send NACK to indicate that no more bytes will be read.
    NACK;

    // Read the data from the I2C data register.
    data = I2C1->D;
    I2C_WAIT

    // Stop the I2C communication.
    I2C_M_STOP;

    // Read data from the I2C data register.
    data = I2C1->D;

    // Return the data read from the specified address of the I2C device.
    return data;
}
/**
 * @brief Writes a byte of data to a specific address of an I2C device.
 *
 * This function performs a complete I2C transaction to write a byte of data
 * to the specified address of the given I2C device.
 *
 * @param dev The I2C device address (7-bit) to communicate with.
 * @param address The register address where the data will be written.
 * @param data The byte of data to be written to the specified address.
 *
 *
 * @reference Alexander G. Dean, "Embedded_Systems_Fundamentals with
 *        ARM Cortex-M based Microcontrollers", chapter 8.
 */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data) {
    // Set I2C to transmit mode.
    I2C_TRAN;

    // Start the I2C communication.
    I2C_M_START;

    // Send the I2C device address for writing.
    I2C1->D = dev;
    I2C_WAIT

    // Send the register address where the data will be written.
    I2C1->D =  address;
    I2C_WAIT

    // Send the byte of data to be written to the specified address.
    I2C1->D = data;
    I2C_WAIT

    // Stop the I2C communication to complete the write operation.
    I2C_M_STOP;
}


