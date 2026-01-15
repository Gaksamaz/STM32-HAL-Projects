/*
 * lcd_i2c.c
 *
 *  Created on: Jul 25, 2025
 *      Author: gaksamaz
 */

#include "lcd_i2c.h"
#include "string.h"

#define LCD_ADDR (0x27 << 1) //PCF8574 default I2C address

extern I2C_HandleTypeDef hi2c1;

void lcd_send_cmd(char cmd)
{
	uint8_t data_u, data_l;
	uint8_t data_t[4];

	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C; //en=1, rs=0
	data_t[1] = data_u | 0x08; // en=0
	data_t[2] = data_l | 0x0C;
	data_t[3] = data_l | 0x08;

	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_t, 4, 100);
}

void lcd_send_data(char data)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];

    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D; // en=1, rs=1
    data_t[1] = data_u | 0x09;
    data_t[2] = data_l | 0x0D;
    data_t[3] = data_l | 0x09;

    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_t, 4, 100);

}

void lcd_init(I2C_HandleTypeDef *hi2c)
{
	HAL_Delay(50);
	lcd_send_cmd(0x30);
	HAL_Delay(5);
	lcd_send_cmd(0x30);
	HAL_Delay(1);
	lcd_send_cmd(0x30);
	lcd_send_cmd(0x20); // 4-bit mode
	lcd_send_cmd(0x28); // 2 lines, 5x8 font
	lcd_send_cmd(0x08); // display off
	lcd_send_cmd(0x01); // clear
	HAL_Delay(2);
	lcd_send_cmd(0x06); // entry mode
	lcd_send_cmd(0x0C); // display on
}

void lcd_send_string(char *str)
{
	while (*str) lcd_send_data(*str++);
}


