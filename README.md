# DIY 3D Mouse for CAD programs

- code on STM32F303K8
- Drivers
  - TLV493D (3-axis magnetometer)
  - W25Q32 (flash)
- 3D Models of mouse
- Windows APP for settings

# About

This project aims to help CAD designers create their own 3D DIY Mouse for a cheaper price than usual.
With a 3D Mouse, a designer is able to model significantly faster than with a classic 2D mouse.
The brain of our project is a microcontroller STM32F303K8, which communicates with a 3D magnetic
sensor TLV493D and transforms its data, so that they can be used in a CAD programme. This, in turn reduces
the amount of movements necessary for certain operations (e.g. rotation of an object).

# Drivers

We created drivers in the StmCubeIDE for all external HW.

## TLV493D

The 3D magnetic sensor TLV493D-A1B6 detects the magnetic flux density in three directions; x, y and z. The
magnetic flux density in the z-direction (Bz) is detected by a lateral-Hall plate parallel to the surface of the
chip. The magnetic flux density in the x- and y-direction (Bx and By) are detected by vertical-Hall plates
perpendicular to the surface of the chip.

<p align="center">
    <img src="img/tlv493d.png" width="" title="GPIO pin block scheme">
</p>

The MCU communicates with the sensor over I2C.

## W25Q32

External serial flash memory W25Q32 (32M-bit) serves as a storage place for user configuration of DPI.
It is organized 16384 programmable pages of 256-bytes each.

<p align="center">
    <img src="img/w25q32.jpg" width="450" title="Flash W25Q32">
</p>

The W25Q32 supports SPI which will be used for communication with the STM32F303K8.

## Sending commands to PC
### Python script
For the proper function of 3D mouse it is necessery to enable python script, which will handle communication with STM32 and application.
STM32 is sending JSON message which consist rotation of 3 axis and if button was pressed with pre-bind action, which can be set in application with sensitivity of mouse movements.
#### Server
Script is creating server which communicated with application via localhost on port 12345 
# Models

Folder models contains .stl model files which are 3D representation of 3D mouse.

# App

The PC app is a WPF GUI, which communicates with the microcontroller via UART trough a IO port.

It provides seamless communication without the user knowing anything about the device.

Since the app uses WPF it is only available on Windows 10 or higher.

## Features

 - Provides basic mouse information (WIP Image)
 - Button mapping (WIP Image)
 - Mouse axis sensitivity settings (WIP Image)
