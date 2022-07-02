#ifndef _SCR_Drive_H
#define _SCR_Drive_H

#include <ArduinoJson.h>

#include "sensesp/system/observable.h"
#include "sensesp/transforms/transform.h"


#define UART_Interfac 1
#define I2C_Interfac 0

#define Data_Interface I2C_Interfac

#define I2C_ADDRESS  0x47

namespace sensesp {
/**
 * 
 */

class ScrDrive : public IntegerTransform {
 public:
    ScrDrive(int canal);
    
    void set_input(int new_value, uint8_t input_channel = 1) override ;

    void SCR_SetMode(uint8_t Mode);
    void SCR_ChannelEnable(uint8_t Channel);
    void SCR_ChannelDisable(uint8_t Channel);
    void SCR_VoltageRegulation(uint8_t Channel,  uint8_t Angle);
    void SCR_GridFrequency(uint8_t Hz);
    void SCR_Reset(uint8_t Delay);
    void SCR_SetBaudrate(uint32_t Baudrate);

 private:
  
};
}  // end namespace

#endif
