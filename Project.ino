//    Arduino code
//    Arduino to Excel using Gobetino
//      - Arduino logs 100 values 
//      - Gobetwino takes these values and puts them in data.txt 
//      - Excel Imports this file as a csv and draws the graph
// ==========< option 1 >========== 
//      FIRST logs data to chip's sram,  THEN sends to pc using Gobetwino  
//      - 1028 bytes can be stored  
//      - readings are taken fast this way 
// ==========< option 2 >========== 
//      log straight to .txt with Gobetwino  
//      - approximately a max of 37 readings/sec , depending on the serial speed you choose
//      - this way is safe (unlimited with the chips 1024 bytes of SRAM)  

#include<Wire.h>

const int MPU_addr=0x68;            //I2C address of the MPU-6050
int16_t Tmp,GyX,GyY,GyZ;

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);                 //PWR_MGMT_1 register
  Wire.write(0);                    //Set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);
}


void loop() 
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);                    //Starting With Register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  //Request a Total of 14 Registers

  Serial.print("#S|ARDTX|["); 
  GyX=Wire.read()<<8|Wire.read();     //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();     //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();     //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  Serial.print(GyX);
  Serial.print(",");
  Serial.print(GyY);
  Serial.print(",");
  Serial.print(GyZ);
  //Serial.print(",");
  delay(3000);
  
  // #S  ->  tells gobetwino to listen
  // |ARDXL|[  ->  Gobetwino runs the ARDXL command I made,  
  // which opens Arduino_Data.xlsx in this app's folder 
  // NOTE: there must already be a Arduino_Data.xlsx in the folder 
  
  Serial.println("]#");
  // # tells Gobetwino to stop paying attention
}
 
