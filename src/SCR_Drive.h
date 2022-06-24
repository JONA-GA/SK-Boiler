#ifndef _SCR_Drive_H
#define _SCR_Drive_H

#include <ArduinoJson.h>

#include "sensesp/system/observable.h"
#include "sensesp/transforms/transform.h"
#include "DEV_Config.h"

#include "sensesp/signalk/signalk_emitter.h"


#define UART_Interfac 1
#define I2C_Interfac 0

#define Data_Interface I2C_Interfac

#define I2C_ADDRESS  0x47

using namespace sensesp;

/**
 * 
 */

class ScrDrive : public IntegerTransform {
 public:
 ScrDrive();
 void set_input(uint8_t new_value, uint8_t input_channel = 0) ;
void SCR_SetMode(UBYTE Mode);
void SCR_ChannelEnable(UBYTE Channel);
void SCR_ChannelDisable(UBYTE Channel);
void SCR_VoltageRegulation(UBYTE Channel,  UBYTE Angle);
void SCR_GridFrequency(UBYTE Hz);
void SCR_Reset(UBYTE Delay);
void SCR_SetBaudrate(UDOUBLE Baudrate);

 private:
  
};

#endif
