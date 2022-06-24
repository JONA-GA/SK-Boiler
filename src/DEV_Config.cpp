#include "DEV_Config.h"

#define I2C_SDA 18
#define I2C_SCL 23

UBYTE IIC_Addr_t = IIC_Addr;
void GPIO_Init()
{
   
}
void Config_Init()
{
  
  
  //i2c config
  Wire.begin(I2C_SDA, I2C_SCL);
}

void DEV_I2C_WriteByte(UBYTE add_, UBYTE data_)
{
  Wire.beginTransmission(IIC_Addr_t);
  Wire.write(add_);
  Wire.write(data_ & 0xFF);
  Wire.endTransmission();
}

void DEV_I2C_WriteWord(UBYTE add_, UWORD data_)
{
  Wire.beginTransmission(IIC_Addr_t);
  Wire.write(add_);
  Wire.write((data_ >> 8) & 0xFF);
  Wire.write(data_ & 0xFF);
  Wire.endTransmission();
}

UBYTE DEV_I2C_ReadByte(UBYTE add_)
{
  Wire.beginTransmission(IIC_Addr_t);
  Wire.write(add_);
  Wire.endTransmission();
  Wire.requestFrom(IIC_Addr_t,(uint8_t) 1 );
  if (Wire.available()) {
   return Wire.read();
  }
  return 0;;
}

UWORD DEV_I2C_ReadWord(UBYTE add_)
{
 uint16_t x; uint16_t t;

  Wire.beginTransmission(IIC_Addr_t);
  Wire.write(add_); 
  Wire.endTransmission();
  Wire.requestFrom(IIC_Addr_t,(uint8_t) 2);
  t = Wire.read();
  x = Wire.read();
  x <<= 8;
  x |= t;
  return x;
}

void DEV_Set_I2CAddress(UBYTE add_)
{
  IIC_Addr_t = add_;
 }


 /******************************************************************************
function: 
  Uart receiving and sending
*****************************************************************************
UBYTE DEV_Uart_ReceiveByte()
{
  while(1){
    if(mySerial.available()){
      return  mySerial.read();
    }
  }
}

void DEV_Uart_SendByte(char data)
{
  mySerial.write(data);
}

void UART_Write_nByte(char *data, UDOUBLE Num)
{
  UDOUBLE i;
  for( i=0; i<Num; i++){
    mySerial.write(data[i]);
  }
}

void DEV_Uart_SendString(char *data)
{
  UWORD i;
  for(i=0; data[i] != '\0'; i++){
    mySerial.write(data[i]);
  }
}

void DEV_Uart_ReceiveString(char *data, UWORD Num)
{  
  UWORD i;
  while(1){
    if(mySerial.available()){
        data[i] =  mySerial.read();
        i++;
        //Serial.print(data[i]);
        if(i >= Num){
            break;
          }
      }
   }

  data[Num-1] = '\0';
}

void DEV_Set_GPIOMode(UWORD Pin, UWORD mode)
{

  if(mode == 1){
    pinMode(Pin, INPUT);
  }
  else if(mode == 0){
    pinMode(Pin, OUTPUT);
  }
}


void DEV_Set_Baudrate(UDOUBLE Baudrate)
{
  mySerial.begin(Baudrate);
}
*/
