#include <Arduino.h>
#include "../lib/DataTransmit/DataTransmit.hpp"
#include "../lib/GPIOStuff/GPIOStuff.hpp"
#include <future>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// put function declarations here:
void IncorrectAnswer();

void setup() {
  // put your setup code here, to run once:
  analogWriteResolution(16);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);

  Serial.begin(9600); //TEMP
}

int currentLevel = 1;
float times[MAX_LEVEL];


void loop() {
  // put your main code here, to run repeatedly:
  timeval startTime;
  gettimeofday(&startTime, NULL);
  
  int sequence[MAX_LEVEL];
  for (int i = 0; i < currentLevel; i++) { //generate random sequence
    int rand = random(1,5); //random between 1 and 4 inclusive
    sequence[i] = rand;
    Serial.println(rand); //TEMP
  }

  //output the sequence here
  for (int i = 0; i < currentLevel; i++) {
    if (sequence[i] == 1) {
      Output(LED_PIN_1, TONE_1, 2000);
    }
    else if (sequence[i] == 2) {
      Output(LED_PIN_2, TONE_2, 2000);
    }
    else if (sequence[i] == 3) {
      Output(LED_PIN_3, TONE_3, 2000);
    }
    else {
      Output(LED_PIN_4, TONE_4, 2000);
    }
    if (i != currentLevel - 1)
      delay(1000);
  }

  //get user inputs
  bool isCorrect = true;
  for (int i = 0; i < currentLevel; i++) {
    bool inputRecieved = false;
    int answer = 0;
    while (!(answer = CheckInputs())) { }
    if (answer != sequence[i]) {
      IncorrectAnswer();
      isCorrect = false;
      break;
    }

  }

  timeval endTime;
  gettimeofday(&endTime, NULL);
  float deltaTime = endTime.tv_sec - startTime.tv_sec;
  //times[currentLevel - 1] = deltaTime; //PROBLEM AREA!!!!!!

  if (isCorrect && currentLevel < MAX_LEVEL) {
    currentLevel++;
    delay(1000);
  }
  else {
    currentLevel = 1;

    // //upload data here
    // for (int i = 0; i < MAX_LEVEL; i++) { //send times per level in secs
    //   AddDataToBuf((uint8_t*)&times[i], sizeof(float));
    // }
    // AddDataToBuf((uint8_t*)&currentLevel, sizeof(int));
    // TransmitData();
  }
}
