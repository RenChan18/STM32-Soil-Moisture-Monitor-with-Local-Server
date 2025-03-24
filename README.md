# STM32-Soil-Moisture-Monitor-with-Local-Server
As part of the project, an IoT device is being developed to measure soil moisture and transmit data to a local server via Wi-Fi. The data can be viewed through a web interface or a mobile application.

## Description

At the moment, the basic functionality has been implemented, which allows:
- Read data from the soil moisture sensor.
- Display this data on the LCD display.
- Send data via UART to a PC.

In the future, the project will be complemented by Wi-Fi connectivity for remote monitoring and management.

## Assembly

To build the project, **libopencm3** is used to work with the STM32 microcontroller. The project uses a Makefile to manage the build process.

1. Clone the repository.
   ```bash
   git clone --recursive git@github.com:RenChan18/STM32-Soil-Moisture-Monitor-with-Local-Server.git
   ```
2. Make sure that you have the following dependencies installed:
- GCC for ARM (arm-none-eabi)
- Make
3. Assemble the project using the command:
   ```bash
   make
   ```

## Connections

Below are the wiring connections between STM32, LCD1602, Soil Moisture Sensor, and TTL-USB Converter. Each module serves a specific purpose within the project.  

- **LCD1602**: Displays information received from the STM32 via I2C communication.  
- **Soil Moisture Sensor**: Measures soil moisture levels and provides analog and digital outputs.  
- **TTL-USB Converter**: Provides serial communication between the STM32 and a computer or other devices.  

---

#### **STM32 & LCD1602 (I2C Communication)**  
| **STM32** | **LCD1602** |
| :-------: | ----------- |
|    GND    | GND         |
|    5V     | VCC         |
|    PB9    | SDA         |
|    PB8    | SCL         |

---

#### **STM32 & Soil Moisture Sensor**  
| **STM32** | **Soil Moisture Sensor** |
| :-------: | --------------- |
|    GND    | GND             |
|    3V     | VCC             |
|    PA0    | A0 (Analog Output) |
|   PB10    | D0 (Digital Output) |

---

#### **STM32F411VET & TTL-USB Converter (Serial Communication)**  
| **STM32F411VET** | **TTL-USB Converter** |
| :--------------: | :-------------------: |
|       TX (PA9)    |          RX           |
|       RX (PA10)   |          TX           |
|       GND         |          GND          |


