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

//BUTTON PINS
const int RESET_SCORES = 40;
const int AWAY_INC = 42;
const int AWAY_DEC = 43;
const int HOME_INC = 44;
const int HOME_DEC = 45;

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
int goal_light = 53; //SIREN GOAL LIGHT

const int IR_RECEIVE_PIN = 2;

//Music
// Define notes with their assigned frequency
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247

#define BUZZER A0 // Your buzzer pin


// Melody notes
int championMelody[] = {

  NOTE_D5, NOTE_CS5, NOTE_D5, NOTE_CS5, NOTE_A4, 0, NOTE_FS4, NOTE_B4, NOTE_FS4,

  NOTE_G3, 0, NOTE_A3, 0,

  NOTE_D5, NOTE_E5, NOTE_FS5, NOTE_A5, NOTE_FS5, NOTE_B4, NOTE_CS5, NOTE_B4,

  NOTE_FS3, NOTE_F3, NOTE_E3, NOTE_C4, NOTE_B3,

  NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, 0, 
  NOTE_G5, NOTE_FS5, NOTE_G5, NOTE_FS5, NOTE_E5, 0, 
  
  NOTE_FS5, 0, NOTE_D5, NOTE_G5, 
  NOTE_FS5, 0, NOTE_D5, NOTE_G5, 
  NOTE_F5, 0, NOTE_D5, NOTE_G5, 
  NOTE_F5, 0, NOTE_D5,

  NOTE_A3, 0, NOTE_A3,

  NOTE_C5, NOTE_A4, NOTE_D5,
};

// Note durations
int championNoteDurations[] = {

  1, 4, 4, 2, 2, 4, 4, 2, 1,

  2, 4, 2, 4,

  1, 4, 4, 2, 2, 4, 4, 1, 
  
  4, 4, 2, 2, 2,

  1, 4, 4, 2, 2, 2, 
  1, 4, 4, 2, 2, 2,

  2, 4, 2, 4,
  2, 4, 2, 4, 
  2, 4, 2, 4, 
  2, 4, 1, 
  
  8, 8, 1,

  4, 8, 1,
};

//==============================================================

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  //init both sets of 7 segment display pins
  for (int i=0; i < 7; i++){
    pinMode(segmentPins[0][i], OUTPUT);
    pinMode(segmentPins[1][i], OUTPUT);
  }

  pinMode(board_light, OUTPUT);
  pinMode(goal_light, OUTPUT);
  pinMode(RESET_SCORES, INPUT_PULLUP);
  pinMode(AWAY_INC, INPUT_PULLUP);
  pinMode(AWAY_DEC, INPUT_PULLUP);
  pinMode(HOME_INC, INPUT_PULLUP);
  pinMode(HOME_DEC, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!isPlayingGame)
  {
    runTest();
    Serial.println("Beginning Game");
    startGame();
  }
  //Reset Button
  if (digitalRead(RESET_SCORES) == 0) {
    delay(50);
    score(1);
  }
  //Away Scores
  else if (digitalRead(AWAY_INC) == 0) {
    delay(50);
    score(0);
  }
  //Away Minus Goal
  else if (digitalRead(AWAY_DEC) == 0) {
    delay(50);
    decreaseScore(0);
  }
  //Home Scores
  else if (digitalRead(HOME_INC) == 0) {
    delay(50);
    score(1);
  }
  //Home Minus Goal
  else if (digitalRead(HOME_DEC) == 0) {
    delay(50);
    decreaseScore(1);
  }
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
  digitalWrite(goal_light, HIGH);
  if (scores[team] == 9) {
    gameover(team);
  }
  else{
    scores[team]++;
    if (isPlayingGame) {
        tone(A0, 100);
        delay(2000);
        noTone(A0);
        delay(1000);
        tone(A0, 100);
        delay(2000);
        noTone(A0);
        tone(A0, 100);
        delay(5000);
        noTone(A0);
    }
  }
  lightSegments(team);
  digitalWrite(goal_light, LOW);
}

//Lights goal light siren for X seconds
void goalLight(int sec) {
  sec = sec * 1000;
  digitalWrite(goal_light, HIGH);
  delay(sec);
  digitalWrite(goal_light, LOW);
}

//when the passed in team wins
void gameover(int team){
  digitalWrite(board_light, HIGH);

  offSegments();
  playChampionMelody();
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
  Serial.println("Resetting");
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
  Serial.println("Starting Test");
  lightSegments[home];
  lightSegments[away];
  delay(400);
  //Serial.println(lightSegments[home].toString());
  for (int i = 0; i < 9; i++){
    score(home);
    digitalWrite(board_light, HIGH);
    delay(200);
    score(away);
    digitalWrite(board_light, LOW);
    delay(200);
    //Serial.println(lightSegments[home].toString());
  }
  delay(400);
  reset();
  loadCircle(5);
  Serial.println("Finishing Test");
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

//Music-related
// Function to play the Queen's melody
void playChampionMelody() {
  playMelody(championMelody, championNoteDurations, 72);
}


// Function to play a melody
void playMelody(int *melody, int *noteDurations, int notesLength) {
  for (int thisNote = 0; thisNote < notesLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
  }
}


