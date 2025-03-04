# STM32-Soil-Moisture-Monitor-with-Local-Server
As part of the project, an IoT device is being developed to measure soil moisture and transmit data to a local server via Wi-Fi. The data can be viewed through a web interface or a mobile application.

## Description

At the moment, the basic functionality has been implemented, which allows:
- Read data from the soil moisture sensor.
- Display this data on the LCD display.

In the future, the project will be complemented by Wi-Fi connectivity for remote monitoring and management.

## Assembly

To build the project, **libopencm3** is used to work with the STM32 microcontroller. The project uses a Makefile to manage the build process.

1. Clone the repository.
2. Make sure that you have the following dependencies installed:
- GCC for ARM (arm-none-eabi)
- Make
3. Assemble the project using the command:
   ```bash
   make
