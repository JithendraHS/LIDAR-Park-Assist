# LIDAR ParkAssist

The LIDAR ParkAssist project is aimed at creating a car reverse parking assistance system using a TF-Luna 8-meter LiDAR sensor and a KL25Z microcontroller board. The system accurately measures the distance between the car and an obstacle using the LiDAR sensor, providing real-time feedback to the driver during reverse parking.

## Project Overview

The project involves the following key components and features:

- **Tasks:** Two FreeRTOS tasks, `forward` and `reverse`, handle the forward and reverse gear logic, respectively. Task 1 monitors for reverse action and triggers Task 2, suspending itself once a forward action is detected.

- **Sensor Integration:** The TFmini LiDAR sensor is connected to the KL25Z board to measure distances. The I2C communication protocol is employed for seamless data exchange between the sensor and the microcontroller.

- **LED Feedback:** An embedded LED on the KL25Z board changes its color from green to red based on the measured distance, providing real-time visual feedback to the driver.

- **State Machine:** The implementation incorporates a state machine to ensure that the system operates exclusively in reverse mode. The state machine smoothly transitions to an LIDAR non reading state state when the car switches to the forward mode.

## Implementation Details

### LIDAR park assist FSM
![LIDAR_park_assist_FSM drawio](https://github.com/JithendraHS/LIDAR-Park-Assist/assets/37045723/e0411bc6-c66e-4ab4-91c0-bbee4e3590ee)

### Tasks

#### `forward`

- Manages the logic associated with the forward gear state.
- Reads touch sensor values, shifts gears between forward and reverse
- Suspends and resumes Task 2 based on gear transitions.

#### `reverse`

- Manages the logic associated with the reverse gear state.
- Reads distance from the TFmini LiDAR sensor and adjusts LED color based on the measured distance.

### Dependencies

- FreeRTOS
- TF-Luna LiDAR Sensor product manual
- KL25Z Microcontroller SDK

## How to Use

1. Connect the TF-Luna LiDAR sensor to the appropriate I2C pins on the KL25Z board.
  - KL25Z PTE1 <----> LIDAR SCK
  - KL25Z PTE0 <----> LIDAR SDA
  - KL25Z GND  <-----> LIDAR I2C mode
2. Build and flash the project to the microcontroller using your preferred IDE.
3. Monitor the LED color changes (green to yellow to red) for real-time feedback during reverse parking.

## Testing

Results can be seen in this video: `LIDAR_park_assist.MP4`

## Demo

Please checkout the demo video: `LIDAR_park_assist_demo.MP4`

## Usage
1. Clone the repository.
2. Open the project in your MCUExpresso IDE.
3. Configure and build the project for your target platform.
4. Flash the binary onto the microcontroller.
5. Run and observe the LED transformation.

## References
1. Took reference and some part of code from: Alexander G. Dean, "Embedded_Systems_Fundamentals with ARM Cortex-M based Microcontrollers," Chapter 8.
2. https://files.seeedstudio.com/wiki/Grove-TF_Mini_LiDAR/res/SJ-PM-TF-Luna-A03-Product-Manual.pdf


