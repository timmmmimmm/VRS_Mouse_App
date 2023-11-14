# DIY 3D Mouse for CAD programs
- code on STM32F303k8
- Drivers
    - TLV493D (3-axis magnetometer)
    - W25Q32 (flash)
- 3D Models of mouse
- Windows APP for settings 
# About 

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
    <img src="img/w25q32.jpg" width="" title="Flash W25Q32">
</p>

The W25Q32 supports SPI which will be used for communication with the STM32F303K8.
## Sending commands to PC 
# Models
Folder models contains .stl model files which are 3D representation of 3D mouse.   
# App
