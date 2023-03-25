#include <LCD_ST7032.h>
LCD_ST7032 lcd;
void setup() {
  lcd.begin();
  lcd.setcontrast(24); //contrast value range is 0-63, try 25@5V or 50@3.3V as a starting value
}
void loop() {
  static int counter = 0;  
  // Write a piece of text on the first line...
  lcd.setCursor(0, 0);  //LINE 1, ADDRESS 0
  lcd.print("Hello World");
  // Write the counter on the second line...
  lcd.setCursor(0, 1);
  lcd.print(counter/10, DEC);
  lcd.write('.');
  lcd.print(counter%10, DEC);
  lcd.write(' ');
  delay(500);
  counter++;
}
