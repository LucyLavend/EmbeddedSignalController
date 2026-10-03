/* 

Embedded Signal Controller
Lucy Jongebloed
Class ELV1-3
25-09-2026

*/
// ----------- Pins ------------------

// Hanze prototyping board pins:
// buttons: 34 and 35
// leds: yellow: 4, blue: 23, green: 22, red 21

const int potPin = 16;
const int morseLEDPin = 9;

const int button1Pin = 4;
const int button2Pin = 5;

const int blueLEDPin = 12;
const int greenLEDPin = 11;
const int redLEDPin = 10;

// ------------ General -----------------

unsigned long uptime = 0;

// assign the pin numbers to color names
enum ledColor : int{
  BLUE = blueLEDPin,
  GREEN = greenLEDPin,
  RED = redLEDPin
};

bool runMorse = false;

bool button1PrevPressed = false;
bool button2PrevPressed = false;

// ------------- Morse ----------------

// 0 = short, 1 = long
bool morseMessage[] = {0,0,0, 1,1,1, 0,0,0};
int messageSize = sizeof(morseMessage) / sizeof(morseMessage[0]);
int currentCharacterIndex = 0;

// wait time in seconds
float morseShortTime = 0.5;
float morseLongTime = 1.0;
float morsePauseTime = 0.5;

// currently selected wait time
int currentWaitTime = 0;

// timer
unsigned long morseTimerStartTime = 0;

// pause between blinks
bool morsePaused = false;

// -----------------------------

void setup() {

  Serial.begin(115200);


  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  pinMode(potPin, INPUT);

  pinMode(morseLEDPin, OUTPUT);

  pinMode(blueLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  initMorse();
}

void loop() {

  // flip input values because they run to ground
  bool button1Pressed = !digitalRead(button1Pin);
  bool button2Pressed = !digitalRead(button2Pin);

  Serial.println(button1Pressed);
  Serial.println(button2Pressed);

  if(button1Pressed && !button1PrevPressed){
    runMorse = true;
    initMorse();
  }
  else if(button2Pressed && !button2PrevPressed){
    runMorse = false;
    digitalWrite(morseLEDPin, LOW);
  }

  button1PrevPressed = button1Pressed;
  button2PrevPressed = button2Pressed;

  if(runMorse){
    runMorseSequence();
  }

  updateStatusLED();

}

// ------ morse -------

void initMorse(){
  // get the wait time for the current character, then multiply by 1000 to get the milliseconds from seconds
  currentWaitTime = (morseMessage[currentCharacterIndex] ? morseLongTime : morseShortTime) * 1000;

  currentCharacterIndex = 0;

  // save the current time
  morseTimerStartTime = millis();
  morsePaused = false;
    digitalWrite(morseLEDPin, HIGH);
}

void runMorseSequence(){

  uptime = millis();

  // called every time timer ends
  if(uptime - morseTimerStartTime > currentWaitTime){


    if(morsePaused){
      currentWaitTime = morsePauseTime * 1000; // multiply to get milliseconds

      digitalWrite(morseLEDPin, LOW);
      morsePaused = false; // unpause next timer loop

    }
    else{
      // get the wait time for the current character, then multiply by 1000 to get the milliseconds from seconds
      currentWaitTime = (morseMessage[currentCharacterIndex] ? morseLongTime : morseShortTime) * 1000;
      digitalWrite(morseLEDPin, HIGH);
      
      if(currentCharacterIndex < messageSize){
        currentCharacterIndex = currentCharacterIndex++;
      }
      else{
        runMorse = false;
      }
      
      morsePaused = true; // pause next time

    }

    morseTimerStartTime = uptime;
    
  }
}

// -------- potentiometer LEDs -------

void updateStatusLED(){

  int potValue = analogRead(potPin);

  // set color of status LED based on potentiometer rotation
  if(potValue <= 1000){
    setStatusLEDColor(ledColor::BLUE);
  }
  else if(potValue > 1000 && potValue <= 2500){
    setStatusLEDColor(ledColor::GREEN);
  } 
  else{
    setStatusLEDColor(ledColor::RED);
  }
  
}

void setStatusLEDColor(int ledCol){
  digitalWrite(blueLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  
  digitalWrite(ledCol, HIGH);
}