/* 

Embedded Signal Controller
Lucy Jongebloed
Class ELV1-3
25-09-2026

*/
// ----------- Pins ------------------

// buttons: 34 and 35
// leds: yellow: 4, blue: 23, green: 22, red 21

const int potPin = 18;
const int ledPin = 48;
const int button1Pin = 4;
const int button2Pin = 5;

// ------------ General -----------------

unsigned long uptime = 0;

bool buttonPressed = false;

int potValue = 0;

// ------------- Morse ----------------

// 0 = short, 1 = long
bool morseMessage[] = {0,0,0, 1,1,1, 0,0,0};
int messageSize = sizeof(morseMessage) / sizeof(morseMessage[0]);
int currentCharacterIndex = 0;

// units in milliseconds
float morseShortTime = 500;
float morseLongTime = 1000;
float morsePause = 500;

bool morseRunning = false;

bool isBlinking = false;

// ----------- States ------------------

enum class State{
  SHORT,
  LONG,
  WAIT,
  IDLE
};

State currentState = State::IDLE;

// ----------- Colors ------------------

struct Color{
  uint r, g, b;
};

namespace colors {
  const Color RED = {255, 0, 0};
  const Color GREEN = {0, 255, 0};
  const Color BLUE = {0, 0, 255};
};

Color currentColor = colors::BLUE;

// -----------------------------

void setup() {

  Serial.begin(115200);

  // pullup because on my PCB I have wired the buttons to ground
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);

}

void loop() {

  for(int i = 0; i < messageSize; i++){
    // i understand how to implement for loops, 
    // they just were not relevant for this project for me
  }

  uptime = millis();

  // state machine
  switch(currentState){
    case State::IDLE:
      
      // check for button press
      break;
    case State::LONG:
      
      break;

  }

  // if(isBlinking){
  //   if(currentCharacterIndex == 0){

  //   }
  //   if((uptime - blinkStartTime) >= morseShortTime){

  //   }
  // }
  unsigned long blinkStartTime = uptime;


  // if(buttonPressed == 1){
  //   blinkLED(0, currentColor);
  // }

  updateColor();

  Serial.println(potValue);
}

void updateColor(){

  potValue = analogRead(potPin);

  // divide 4096 into 3 segments for LED colors
  if(potValue <= 1 * (4096/3)){
    currentColor = colors::BLUE;
  }
  else if(potValue <= 2 * (4096/3)){
    currentColor = colors::GREEN;
  } 
  else{
    currentColor = colors::RED;
  }

  
}

void blinkLED(bool length, Color color){

  length;
  // use rgbLedWrite for the built-in RGB LED of the ESP32 S3
  rgbLedWrite(ledPin, color.r, color.g, color.b);

}