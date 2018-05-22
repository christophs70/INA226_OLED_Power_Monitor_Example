#include <SPI.h>                                                               // OLED Display Drivers (by Adafruit
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <INA226.h>                                                            // INA226 Library (by Zanshin)






#define OLED_RESET 4
Adafruit_SSD1306 Display(OLED_RESET);                                          // Construct a display object named "Display"

INA226_Class INA226;                                                           // Construct a power monitor object names "ina"

int32_t  rawCurrent = -666;
uint16_t rawVoltage =  666;
int32_t  rawWatts   = -666;

String output  = String(8);
String current = String(8);
String voltage = String(8);
String watts   = String(8);
String temp    = String(8);
uint8_t x      = 0;

bool negativeNumber = 0;







void setup() {


  Display.begin(SSD1306_SWITCHCAPVCC , 0x3C);                                 // initialise Display and set address to 3C
  Display.display();                                                          // Show Adafruit Logo
  delay(2000);
  Display.clearDisplay();


  INA226.begin(1,100000);                                                     // Begin calibration for an expected ±1 Amps maximum current and for a 0.1Ohm resistor
  INA226.setAveraging(10);                                                     // Average each reading n-times
  INA226.setBusConversion();                                                  // Maximum conversion time 8.244ms
  INA226.setShuntConversion();                                                // Maximum conversion time 8.244ms
  INA226.setMode(INA_MODE_CONTINUOUS_BOTH);                                   // Bus/shunt measured continuously



  delay(20);


}








void loop() {

  rawCurrent = INA226.getBusMicroAmps()  ;
  rawVoltage = INA226.getBusMilliVolts() ;
  rawWatts   = INA226.getBusMicroWatts() ;

  current = divideBy1000( String(rawCurrent , DEC) );
  voltage = divideBy1000( String(rawVoltage , DEC) );

  temp    =               String(rawWatts   , DEC)  ;                // Divide raw microwatts figure by 1000 and discard decimals
  x       = temp.length();
  temp.remove(x-3);
  watts   = divideBy1000(temp);                                      // Divide milliwats by 1000 and keep decimals




  Display.setTextSize(1);
  Display.setTextColor(WHITE , BLACK);                               // Write current data (font color, backgr color)

  Display.setCursor(0,20);
  //Display.print("I = ");
  Display.print( current );
  Display.print(" mA     ");                                         // Trailing spaces to ensure we don't have garbage left over from longer previous print

  Display.setCursor(75,20);
  //Display.print("U= ");
  Display.print( voltage );
  Display.print(" V   ");

  Display.setCursor(45,4);
  Display.print( watts );
  Display.print(" W    ");

  Display.display();


  delay(20);
}













String divideBy1000(String input) {

  output = "";

  if ( input.startsWith("-") ) { output.concat("-");
                                 input.remove(0,1);
                               }

  switch( input.length() ) {
                               case 0 :  {                                                  // 0 character string is fed
                                         return "NaN";
                               }break;

                               case 1 : {                                                   // 1 -> .001
                                        output.concat( ".00" );
                                        output.concat( input );
                               }break;

                               case 2 : {                                                   // 12 -> .012
                                        output.concat( ".0" );
                                        output.concat( input );
                               }break;

                               case 3 : {                                                   // 123 -> .123
                                        output.concat( "." );
                                        output.concat( input );
                               }break;

                               case 4 : {                                                   // 1234 -> 1.234
                                        output.concat( input.substring(0,1) );
                                        output.concat( "." );
                                        output.concat( input.substring(1)   );
                               }break;

                               case 5 : {                                                   // 12345 -> 12.345
                                        output.concat( input.substring(0,2) );
                                        output.concat( "." );
                                        output.concat( input.substring(2)   );
                               }break;

                               case 6 : {                                                    // 123456 -> 123.456
                                        output.concat( input.substring(0,3) );
                                        output.concat( "." );
                                        output.concat( input.substring(3)   );
                               }break;

                              }

  return output;

}
