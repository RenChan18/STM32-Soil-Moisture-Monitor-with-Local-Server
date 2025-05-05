# STM32-Soil-Moisture-Monitor-with-Local-Server
As part of the project, an IoT device is being developed to measure soil moisture and transmit data to a local server via Wi-Fi. The data can be viewed through a web interface or a mobile application.

Here’s the English translation of your project description and build instructions:

---

## Description  

The current implementation includes core functionality that enables:  
- Reading soil moisture sensor data.  
- Displaying this data on an LCD screen (*temporarily unavailable after switching to FreeRTOS; implementation exists in earlier versions*).  
- Sending logs via UART to a PC (*timestamps are currently static; RTC module integration pending*).  

Future updates will introduce Wi-Fi connectivity for remote monitoring and control.  

## Building  

The project uses **libopencm3** for STM32 microcontroller support and a **Makefile** for build automation.  

1. Clone the repository:  
   ```bash  
   git clone --recursive git@github.com:RenChan18/STM32-Soil-Moisture-Monitor-with-Local-Server.git  
   ```  
2. Ensure the following dependencies are installed:  
   - ARM GCC (`arm-none-eabi`)  
   - Make  
   - FreeRTOS  
3. Build the project:  
   ```bash  
   make  
   ```  
4. Flash the firmware to the microcontroller:  
   ```bash  
   make flash  
   ```  
5. Monitor logs using `minicom`:  
   ```bash  
   sudo minicom -D /dev/ttyUSB0 -b 9600 -o -L  
   ```  

---

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


