/*
 * lcd_i2c.h
 *
 *  Created on: Jul 25, 2025
 *      Author: gaksamaz
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include "stm32f1xx_hal.h"

void lcd_init(I2C_HandleTypeDef *hi2c);
void lcd_send_string(char *str);


#endif /* INC_LCD_I2C_H_ */
