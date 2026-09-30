/* 

Embedded Signal Controller
Lucy Jongebloed
Class ELV1-3
25-09-2026

*/
// ----------- Pins ------------------

// buttons: 34 and 35
// leds: yellow: 4, blue: 23, green: 22, red 21

const int potPin = 33;
const int morseLEDPin = 4;

const int button1Pin = 34;
const int button2Pin = 35;

const int blueLEDPin = 23;
const int greenLEDPin = 22;
const int redLEDPin = 21;

// ------------ General -----------------

unsigned long uptime = 0;

bool buttonPressed = false;

int potValue = 0;

enum ledColor : int{
  BLUE = blueLEDPin,
  GREEN = greenLEDPin,
  RED = redLEDPin 
};


// ------------- Morse ----------------

// 0 = short, 1 = long
bool morseMessage[] = {0,0,0, 1,1,1, 0,0,0};
int messageSize = sizeof(morseMessage) / sizeof(morseMessage[0]);
int currentCharacterIndex = 0;

// wait time in seconds
float morseShortTime = 0.5;
float morseLongTime = 1.0;
float morsePauseTime = 0.5;

bool morseRunning = false;

// timer
unsigned long morseTimerStartTime = 0;
bool morsePaused = false;
bool morseStarted = false;

// -----------------------------

void setup() {

  Serial.begin(115200);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  pinMode(potPin, INPUT);

  pinMode(blueLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

}

void loop() {

  runMorseSequence();

  updateStatusLED();

}

void runMorseSequence(){

  uptime = millis();

  // Get the wait time for the current character, then multiply by 1000 to get the milliseconds from seconds
  int waitTime = (morseMessage[currentCharacterIndex] ? morseLongTime : morseShortTime) * 1000;
  
  if(!morseStarted){
    digitalWrite(morseLEDPin, HIGH);
    morseStarted = true;
  } 
  
  if((uptime - morseTimerStartTime) > waitTime && !morsePaused){
    
    // fire when character timer ends:
    digitalWrite(morseLEDPin, LOW);

    // start pause timer
    morsePaused = true;
  }
  else if((uptime - morseTimerStartTime) > morsePauseTime){
    
    // fire when pause timer ends:
    digitalWrite(morseLEDPin, HIGH);

    // increase current character index
    if(currentCharacterIndex < (messageSize -1)){
      currentCharacterIndex++;
    }else{
      currentCharacterIndex = 0;
    }

    // restart morse timer
    morseTimerStartTime = uptime;
    morsePaused = false;
  }

  
  Serial.println(currentCharacterIndex);
  Serial.println(waitTime);


}

void updateStatusLED(){

  potValue = analogRead(potPin);

  // 
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