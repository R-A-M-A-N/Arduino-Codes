#include <OneWire.h>
#include <SPI.h>
#include <SD.h>

File myfile;
const int cs=4;

OneWire ds(2);  // on pin 2 //DS18S20 Temperature chip i/o

void setup() 
{
  Serial.begin(9600);
  Serial.println("Initializing card...");
  if (!SD.begin(cs)) 
  {
    Serial.println("Initialization of the SD card failed!");
    return;
  }
  Serial.println("Initialization of the SD card is done.");
}

void loop(void) 
{
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole;
  uint16_t Fract;
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  ds.reset_search();
  if(!ds.search(addr)) 
  {
    //Serial.print("No more addresses.\n");
    ds.reset_search();
    return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for(i=0;i<9;i++)            // we need 9 bytes
  {
    data[i] = ds.read();
   // Serial.print(data[i], HEX);
   // Serial.print(" ");
  }
   
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100;// % 100;
   
  Serial.print(Whole);
  Serial.print(".");
  Serial.print(Fract);
  Serial.print('\n');
  delay(500);
  /*
  File f;
  f = SD.open("tp.txt", FILE_WRITE);  // Creates a text file
  f.close();
  
  f = SD.open("tp.txt", FILE_WRITE);  // Opens text file tp.txt
  if (f)
  {
    Serial.println("Writing to the text file...");
    f.print(Whole);
    f.print('.');
    f.print(Fract);
    f.println();
    f.close(); // close the file:
    Serial.println("done closing.");
  } 
  else
  {
    Serial.println("error opening the text file!");
  }
  
  //re-open the text file for reading:
  f = SD.open("tp.txt");
  if (f)
  {
    Serial.println("tp.txt:");
    // read all the text written on the file
    while (f.available()) 
    {
      Serial.write(f.read());//>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    }
    f.close();
  } else 
  {
    // if the file didn't open, report an error:
    Serial.println("error opening the text file!");
  }*/
}






    
 
