MPU6050 STM32F103 Project

Reading accelerometer data from the MPU6050 sensor via I2C using an STM32F103 Blue Pill and transmitting the data to a computer via USB CDC.

What Is It Used For?

This project reads 3-axis (X, Y, Z) acceleration data from the MPU6050 accelerometer sensor and sends it to a computer over USB. When you move the sensor, you can observe real-time acceleration values on the terminal screen.

Hardware

STM32F103C8T6 (Blue Pill)

MPU6050 (Accelerometer sensor)

ST-Link V2 (for programming)

Core/
├── Inc/
│   ├── mpu6050.h       # MPU6050 sürücü header
│   └── i2c.h           # I2C konfigürasyon
└── Src/
    ├── main.c          # Ana program
    ├── mpu6050.c       # MPU6050 sürücü
    └── i2c.c           # I2C başlatma
