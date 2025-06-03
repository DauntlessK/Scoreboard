#include <IRremote.hpp>

//team # refs
int away = 0;
int home = 1;

//holds away and home scores, respectively
int scores[2] = {-1,-1};

bool isPlayingGame = false;

//7 segment pin #s
int segmentPins[2][7] = {
  {22,24,26,28,30,32,34},   //away segments A-G
  {23,25,27,29,31,33,35}    //home segments A-G
};

//segment on or off, from segment A to G
int ledNumArray[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},     // 0
    {0, 1, 1, 0, 0, 0, 0},     // 1
    {1, 1, 0, 1, 1, 0, 1},     // 2
    {1, 1, 1, 1, 0, 0, 1},     // 3
    {0, 1, 1, 0, 0, 1, 1},     // 4
    {1, 0, 1, 1, 0, 1, 1},     // 5
    {1, 0, 1, 1, 1, 1, 1},     // 6
    {1, 1, 1, 0, 0, 0, 0},     // 7
    {1, 1, 1, 1, 1, 1, 1},     // 8
    {1, 1, 1, 1, 0, 1, 1}      // 9
};

int ledAlphabetArray[26][7] = {
    {1, 1, 1, 1, 1, 1, 0},     // A
    {0, 1, 1, 0, 0, 0, 0},     // b
    {1, 1, 0, 1, 1, 0, 1},     // C
    {1, 1, 1, 1, 0, 0, 1},     // d
    {0, 1, 1, 0, 0, 1, 1},     // E
    {1, 0, 1, 1, 0, 1, 1},     // F
    {1, 0, 1, 1, 1, 1, 1},     // g
    {1, 1, 1, 0, 0, 0, 0},     // H
    {1, 1, 1, 1, 1, 1, 1},     // I
    {1, 1, 1, 1, 0, 1, 1},     // J
    {1, 1, 1, 1, 1, 1, 0},     // k
    {0, 1, 1, 0, 0, 0, 0},     // L
    {1, 1, 0, 1, 1, 0, 1},     // M---
    {1, 1, 1, 1, 0, 0, 1},     // n
    {0, 1, 1, 0, 0, 1, 1},     // o
    {1, 0, 1, 1, 0, 1, 1},     // P
    {1, 0, 1, 1, 1, 1, 1},     // q
    {1, 1, 1, 0, 0, 0, 0},     // R
    {1, 0, 1, 1, 0, 1, 1},     // S
    {1, 1, 1, 1, 0, 1, 1},     // t
    {1, 1, 1, 1, 1, 1, 0},     // u
    {0, 1, 1, 0, 0, 0, 0},     // V---
    {1, 1, 0, 1, 1, 0, 1},     // w
    {1, 1, 1, 1, 0, 0, 1},     // x---
    {0, 1, 1, 0, 0, 1, 1},     // y
    {1, 0, 1, 1, 0, 1, 1},     // z
};

int board_light = 13;
int goal_light = 53;

const int IR_RECEIVE_PIN = 2;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  //Start Receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  //init both sets of 7 segment display pins
  for (int i=0; i < 7; i++){
    pinMode(segmentPins[0][i], OUTPUT);
    pinMode(segmentPins[1][i], OUTPUT);
  }

  pinMode(board_light, OUTPUT);
  pinMode(goal_light, OUTPUT);
}

//checks ledNumArray (array row (score) and segment (column)) for correct segment lighting then loops through to light each segment
void lightSegments(int team){
  //loop through A-G segments
  for (int i = 0; i < 7; i++){
    //if array value is 1 
    if (ledNumArray[scores[team]][i] == 1){
      digitalWrite(segmentPins[team][i], HIGH);
    }
    //if array value is 0 (segment off)
    else {
      digitalWrite(segmentPins[team][i], LOW);
    }
  }
}

void score(int team){
  //check if this pushes the score over 9
  if (scores[team] == 9) {
    gameover(team);
  }
  else{
    scores[team]++;
  }
  lightSegments(team);
}

//when the passed in team wins
void gameover(int team){
  digitalWrite(goal_light, HIGH);
  digitalWrite(board_light, HIGH);

  offSegments();
  winCircle(10, team);
  reset();
}

void startGame() {
  reset();
  isPlayingGame = true;

  lightSegments(home);
  lightSegments(away);
}

//turn off all segments
void offSegments() {
  //set all segments to off
  for (int i=0; i < 7; i++) {
    digitalWrite(segmentPins[0][i], LOW);
    digitalWrite(segmentPins[1][i], LOW);
  }
}

//reset scores to 0 and turns all LEDS off - sets isPlaying Boolean to false, exits game
void reset(){
  offSegments();
  isPlayingGame = false;

  digitalWrite(goal_light, LOW);
  digitalWrite(board_light, LOW);

  scores[0] = 0;
  scores[1] = 0;
}

//decreases the score by one
void decreaseScore(int team) {
  if (scores[team] != 0) {
    scores[team]--;
    lightSegments(team);
  }
}

//runs through all ints 0-9 to test 7 segment displays
void runTest(){
  //step through all possible scores, 0-9
  reset();
  lightSegments[home];
  lightSegments[away];
  delay(400);
  //Serial.println(lightSegments[home].toString());
  for (int i = 0; i < 9; i++){
    score(home);
    digitalWrite(board_light, HIGH);
    delay(400);
    score(away);
    digitalWrite(board_light, LOW);
    delay(400);
    //Serial.println(lightSegments[home].toString());
  }
  delay(400);
  reset();
  loadCircle(5);
  reset();
}

//loops through all outer segments, creating a loading-type circle
void loadCircle(int times){
  for (int j = 0; j < times; j++){
    for (int i = 0; i < 6; i++){
    digitalWrite(segmentPins[0][i], HIGH);
    digitalWrite(segmentPins[1][i], HIGH);
    delay(150);
    digitalWrite(segmentPins[0][i-1], LOW);
    digitalWrite(segmentPins[1][i-1], LOW);
    if (i == 0 || j == times-1){
      digitalWrite(segmentPins[0][5], LOW);
      digitalWrite(segmentPins[1][5], LOW);
    }
  }
  }
}

//loops through all outer segments, creating a loading-type circle
void winCircle(int times, int team){
  scores[team] = 0;
  lightSegments(team);
  for (int j = 0; j < times; j++){
    for (int i = 0; i < 6; i++){
    digitalWrite(segmentPins[team][i], HIGH);
    delay(150);
    digitalWrite(segmentPins[team][i-1], LOW);
    if (i == 0 || j == times-1){
      digitalWrite(segmentPins[team][5], LOW);
    }
  }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //score(home);
    if (IrReceiver.decode()) {

        /*
         * Print a summary of received data
         */
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
        } else {
            IrReceiver.resume(); // Early enable receiving of the next IR frame
            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
        }

        switch (IrReceiver.decodedIRData.command) {
          case 0x45:
            if (!isPlayingGame) {
              startGame();
            }
            break;
          case 0x44:
            score(home);
            break;
          case 0x77:
            decreaseScore(home);
            break;
          case 0x43:
            score(away);
            break;
          case 0x9:
            decreaseScore(away);
            break;
          case 0x19:
            reset();
            break;
          case 0x46:
            loadCircle(5);
            delay(1000);
            runTest();
            break;
        }

    }
  //delay(3000);
  //runTest();
}
