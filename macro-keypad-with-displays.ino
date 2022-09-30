#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// LCD display for configs/presets
LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte ROWS = 3; //four rows
const byte COLS = 2; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2'},
  {'3','4'},
  {'5','6'}
};
byte rowPins[ROWS] = {9, 5, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// keep track of config position
int configPosition = 1;

void setup() {
  Serial.begin(9600);
  
  for (int i=-1; i <= 3; i++) {
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    TCA9548A(i);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
  }

  TCA9548A(4);
  lcd.init();
  lcd.backlight();
}

void loop() {
  char customKey = customKeypad.getKey();

  if (customKey == '1') {
    if (configPosition == 1) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(142); // "c" key
      delay(100);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(142);
    }

    if (configPosition == 2) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(109); // "m" key
      delay(100);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(109);
    } 
  }

  else if (customKey == '2') {
      if (configPosition == 1) {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(118); // "v" key 
        delay(100);
        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release(118);
      }

      if (configPosition == 2) {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(111); // "0" key
        delay(100);
        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release(KEY_LEFT_SHIFT);
        Keyboard.release(111);
      }
  }

  else if (customKey == '3') {
    if (configPosition == 1) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(65); // "a" key 
      delay(100);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(65);
    }

    if (configPosition == 2) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(101); // "e" key
      delay(100);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(101);
    } 
      
  }

  else if (customKey == '4') {
      if (configPosition == 1) {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(122); // "z" key 
        delay(100);
        Keyboard.release(KEY_LEFT_CTRL);
        Keyboard.release(122);
      }

      if (configPosition == 2) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(104); // "h" key
      delay(100);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(104);
    } 
  }

  else if (customKey == '5') {
    configPosition--;
    
    if (configPosition > 3) {
      configPosition = 1;
    }
    else if (configPosition < 1) {
      configPosition = 3;
    }
    
    TCA9548A(4);
    lcd.setCursor(0, 1);
    lcd.print("                    ");
  }

  else if (customKey == '6') {
    configPosition++;
    
    if (configPosition > 3) {
      configPosition = 1;
    }
    else if (configPosition < 1) {
      configPosition = 3;
    }
    
    TCA9548A(4);
    lcd.setCursor(0, 1);
    lcd.print("                    ");
  }
  
  configPositionManager(configPosition);
}

void configPositionManager(int pos) {
  TCA9548A(4);
  lcd.setCursor(0, 0);
  lcd.print("PRESET");
  
  if (pos == 1){
    drawLCD("General");
    setConfigOne();
  }

  if (pos == 2) {
    drawLCD("Microsoft Teams");
    setConfigTwo();
  }

  if (pos == 3) {
    drawLCD("Random");
    setConfigThree();
  }
}

void setConfigOne(void) {
  drawOled(0, "PASTE");
  drawOled(1, "SELECT ALL");
  drawOled(2, "UNDO");
  drawOled(3, "COPY");
}
void setConfigTwo(void) {
  drawOled(0, "TOGGLE CAMERA");
  drawOled(1, "SHARE SCREEN");
  drawOled(2, "LEAVE MEETING");
  drawOled(3, "TOGGLE MIC");
}

void setConfigThree(void) {
  drawOled(0, "RANDOM 2");
  drawOled(1, "RANDOM 3");
  drawOled(2, "RANDOM 4");
  drawOled(3, "RANDOM 1");
}


void drawOled(int dis, String text) {
  // the order for display may need to be modified based on wiring
  // currently:
  //TCA9548A(0) = 2nd OLED
  //TCA9548A(0) = 3rd OLED
  //TCA9548A(0) = 4th OLED
  //TCA9548A(0) = 1st OLED

  TCA9548A(dis);
  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print(text);
}


void drawLCD(String text) {
  TCA9548A(4);
  lcd.setCursor(0, 1);
  lcd.print(text);
}

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}
