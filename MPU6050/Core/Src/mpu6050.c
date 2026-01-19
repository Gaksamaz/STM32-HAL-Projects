#include "mpu6050.h"

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t check; // Value to read WHO_AM_I register
    uint8_t data;  // Value to be written to registers
    HAL_StatusTypeDef ret; // Return value of HAL functions (success or failure)

    // First, reset the device
    data = 0x80;
    HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1, &data, 1, 1000);
    HAL_Delay(100);
    /* HAL_I2C_Mem_Write() Parameters:
       - hi2c: Pointer to I2C handle
       - MPU6050_ADDR: I2C address of MPU6050 (0xD0 = 0x68 << 1)
       - MPU6050_PWR_MGMT_1: Power management register (address: 0x6B)
       - 1: Register address size (1 byte)
       - &data: Pointer to data to be written
       - 1: Number of bytes to write (1 byte)
       - 1000: Timeout duration (1000 ms = 1 second)
    */

    // Exit sleep mode (after reset, MPU6050 starts in sleep mode)
    data = 0x00;
    ret = HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1, 1, &data, 1, 1000); // Write 0x00 to PWR_MGMT_1 to disable sleep mode
    if (ret != HAL_OK) return 1; // Return error code 1 if write fails
    HAL_Delay(10);
    /* HAL status codes:
       HAL_OK      = success (enum value 0)
       HAL_ERROR   = error   (enum value 1)
       HAL_BUSY    = busy    (enum value 2)
       HAL_TIMEOUT = timeout (enum value 3)
    */

    // WHO_AM_I check
    ret = HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I, 1, &check, 1, 1000);
    if (ret != HAL_OK) return 2;

    // MPU6000 = 0x68, MPU6050 = 0x68, MPU9150 = 0x68
    if (check != 0x68)
    {
        return 3;  // Wrong chip ID
    }

    // Set accelerometer range to ±2g
    data = 0x00;
    ret = HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 1, &data, 1, 1000);
    if (ret != HAL_OK) return 4; // Accelerometer config error
    HAL_Delay(10);

    return 0;  // Success
    /* Error Code Summary:

       0: Success
       1: PWR_MGMT_1 write error (failed to exit sleep mode)
       2: WHO_AM_I read error (I2C communication problem)
       3: Incorrect WHO_AM_I value (device is not MPU6050)
       4: ACCEL_CONFIG write error
    */
}

void MPU6050_Read_Accel(I2C_HandleTypeDef *hi2c, MPU6050_Data *data)
{
    uint8_t raw_data[6]; // 6 bytes of raw data (2 bytes each for x, y, z)
    HAL_StatusTypeDef ret; // Return value of HAL function

    // Read 6 bytes starting from ACCEL_XOUT_H
    ret = HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, raw_data, 6, 1000);

    if (ret == HAL_OK)
    {
        // Combine high and low bytes
        data->accel_x = (int16_t)(raw_data[0] << 8 | raw_data[1]);
        data->accel_y = (int16_t)(raw_data[2] << 8 | raw_data[3]);
        data->accel_z = (int16_t)(raw_data[4] << 8 | raw_data[5]);
        /* Step by step:

           1. raw_data[0] << 8 : Shift HIGH byte left by 8 bits
              raw_data[0] = 0x12
              0x12 << 8 = 0x1200

           2. | raw_data[1] : OR with LOW byte
              0x1200 | 0x34 = 0x1234

           3. (int16_t) : Convert to signed 16-bit integer
              0x1234 = 4660  (positive)
              0xFFFF = -1    (negative)

           Example calculation:
              raw_data[0] = 0xFF (HIGH byte)
              raw_data[1] = 0x00 (LOW byte)

              0xFF << 8  = 0xFF00
              0xFF00 | 0x00 = 0xFF00

              (int16_t)0xFF00 = -256
        */
    }
    else
    {
        // Return zeros in case of error
        data->accel_x = 0;
        data->accel_y = 0;
        data->accel_z = 0;
    }
}
