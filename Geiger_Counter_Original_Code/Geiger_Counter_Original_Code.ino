#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Add the 1602I2C library from the library manager
#include <SPI.h>
#define LOG_PERIOD 15000
#define MAX_PERIOD 60000

unsigned long counts; // GM variable
unsigned long cpm; // CPM variable
unsigned int multiplier; //Set conversion variables
unsigned long previousMillis; //measure time
float usv;

LiquidCrystal_I2C lcd(0x27, 16, 2); //Set the LCD address to 0x27 (1602 monitor)

void tube_impulse() { //Self-increasing
   counts++;
   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
   delay(50); //This could be better implemented...
   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW 
}

void setup()
{
   counts = 0;
   cpm = 0; //count
   multiplier = MAX_PERIOD / LOG_PERIOD; //Calculate multiplier, depends on the period
   Serial.begin(9600);
   attachInterrupt(0, tube_impulse, FALLING); //Interrupt is triggered by falling edge
 
   ///////////////////
   lcd.init();
   // LEDInit
   pinMode(LED_BUILTIN, OUTPUT);
 
   // Output data to the screen
   lcd.backlight();
   lcd.setCursor(3, 0);
   lcd.print("Rawr... x3"); //Write a boot loading screen and provide a Geiger tube startup time.
   lcd.setCursor(0, 1);
   for(int i=0;i<15;i++)
   {
   lcd.write(0xff);
   delay(1000);
   }
}
 
void loop()
{
   unsigned long currentMillis = millis();
   if (currentMillis - previousMillis > LOG_PERIOD) {
     previousMillis = currentMillis;
     cpm = counts * multiplier;//Get the technical times
     Serial.println(counts*4); //Print to serial
     usv = float(cpm) / 151;//Introduce the formula to calculate the radiation intensity

     lcd.clear();
     lcd.print("CPM:");
     lcd.print(cpm);//Output cpm value
     lcd.setCursor(0, 1);//Second line
     lcd.print(usv);
     lcd.print(" uSv/h");//Output intensity value
     counts = 0;//reset

     if (usv >= 10)
     {
       lcd.setCursor(9, 0);
       lcd.print("GTFO!!!");//If the radiation is greater than 10, the "Danger" text is displayed
       delay(0.1);
     }

     else if (usv < 10 && usv >= 0.52)
       {
         lcd.setCursor(9, 0);
         lcd.print("Unsafe");//Display "Unsafe" Text between 0.52-10
         delay(0.1);
       }

       else if (usv < 0.52)
         {
           lcd.setCursor(9, 0);
           lcd.print("Safe :3");//Display a "Safe" text under .52
           delay(0.1);
         }
       }
}
