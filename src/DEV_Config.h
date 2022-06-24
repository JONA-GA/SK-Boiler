#include "Arduino.h"
#include <stdint.h>
#include <stdio.h>
#include <Wire.h>

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
* GPIO 
**/


/**
 * GPIO read and write
**/
#define DEV_Digital_Write(_pin, _value) digitalWrite(_pin, _value == 0? LOW:HIGH)
#define DEV_Digital_Read(_pin) digitalRead(_pin)

/**
* IIC 
**/
#define IIC_Addr             (uint8_t) 0x47

/**
 * delay x ms
**/
#define DEV_Delay_ms(__xms)    delay(__xms)

/**
 * PWM_BL
**/

/*-----------------------------------------------------------------------------*/
void Config_Init();

void DEV_Set_I2CAddress(UBYTE Add);
void DEV_I2C_WriteByte(UBYTE add_, UBYTE data_);
void DEV_I2C_WriteWord(UBYTE add_, UWORD data_);
UBYTE DEV_I2C_ReadByte(UBYTE add_);
UWORD DEV_I2C_ReadWord(UBYTE add_);

/*
UBYTE DEV_Uart_ReceiveByte(void);
void DEV_Uart_SendByte(char data);
void DEV_Uart_SendString(char *data);
void DEV_Uart_ReceiveString(char *data, UWORD Num);
void DEV_Set_Baudrate(UDOUBLE Baudrate);
void UART_Write_nByte(char *data, UDOUBLE Num);

void DEV_Set_GPIOMode(UWORD Pin, UWORD mode);
*/